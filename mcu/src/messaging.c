#include <avr/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "comms.h"
#include "buffer.h"

#define RSTHUM		PORTB6
#define MODEIND		PORTB7

#define CRESP		PORTD2
#define PB			PORTD3
#define CMD			PORTD5
#define SLEEPMODE	PORTD7

#define PWR			PORTE1

#define MaxNetworkSize	5
#define MAX_NUB_CON		3
#define MAX_PHONE_CON	3


extern myDSN;
extern networkStructure;

// Connections to each specific NUB
struct localConnections{
	uint8_t deviceDSN[4];
	uint8_t numOfNubCon;
	uint8_t numOfPhoneCon;			// How many devices are connected to this one
	uint8_t nubConnections[3][4];	// The DSN addresses of the connected devices
	uint8_t phoneConnections[3][6];	// The mac addresses of connected phones
};


// Overall network structure, 175 Bytes
struct networkStructure{
	uint8_t numOfDevices;
	
	struct localConnections device[5];		// Max network size of 5 connections
	
};

extern networkptr;


uint8_t handleMessages(uint8_t *myDSN, struct networkStructure network){
	
	
	uint8_t msgType = (uint8_t) getChar(Transceiver);		// Message type is stored as the first byte
	uint8_t sourceDSN[4];
	// Message that HUMPRO received over air
	
	
	if (msgType == 0x01){
		// [msgType (1 byte), deviceNumInPath (1 byte), msgPathSize (1 byte), msgSize (1 byte), 
		//  msgPath (var), destination phone address (6 bytes), source phone address (6 bytes), message (var)]
		
		uint8_t deviceNumInPath = getChar(Transceiver) + 1;		// Increment to this device
		uint8_t msgPathSize = getChar(Transceiver);
		uint8_t msgSize = getChar(Transceiver);
		uint8_t msg[msgSize];
		uint8_t msgPath[msgPathSize*4];
		for (uint8_t i = 0; i < msgPathSize*4; i ++){
			msgPath[i] = getChar(Transceiver);
		}
		
		uint8_t destPhoneAdd[6];
		for (uint8_t i = 0; i < 6; i ++){
			destPhoneAdd[i] = getChar(Transceiver);
		}
	
		uint8_t srcPhoneAdd[6];
		for (uint8_t i = 0; i < 6; i ++){
			srcPhoneAdd[i] = getChar(Transceiver);
		}
		for (uint8_t i = 0; i < msgSize; i++){
			msg[i] = getChar(Transceiver);
		}
		
		// Seeing if the this device is the last in the path, if so send message data to esp to be transmitted to the phone
		if (deviceNumInPath == msgPathSize){
			uint8_t espMsg[] = {msgType,destPhoneAdd,srcPhoneAdd,msgSize,msg};
			TXWrite(espMsg,sizeof(espMsg),WiFi);
		}
		
		// If not pass along the message to the next NUB device
		else{
			uint8_t msgHeader[] = {msgType,deviceNumInPath,msgPathSize,msgSize};
	
			writeDestDSN(msgPath[deviceNumInPath + 1]);		// The next devices DSN
	
			// Send [msgType (1 byte), deviceNumInPath (1 byte), msgPathSize (1 byte), msgSize (1 byte), 
			//       msgPath (var), destination phone address (6 bytes), source phone address (6 bytes), message (var)]
	
			TXWrite(msgHeader,sizeof(msgHeader),Transceiver);
			TXWrite(msgPath,msgPathSize*4,Transceiver);
			TXWrite(destPhoneAdd,6,Transceiver);			// mac address
			TXWrite(srcPhoneAdd,6,Transceiver);			// mac address
	
			TXWrite(msg, msgSize,Transceiver);
		
			sendAck(myDSN,sourceDSN);
			}
	}
	if (msgType == 0x03){												// Network adjustment (broadcast)
		sourceDSN[0] = (uint8_t) getChar(Transceiver);
		sourceDSN[1] = (uint8_t) getChar(Transceiver);
		sourceDSN[2] = (uint8_t) getChar(Transceiver);
		sourceDSN[3] = (uint8_t) getChar(Transceiver);
		
	}
	
	// Acknowledgment received
	if(msgType == 0x04){
		TXWrite(0x16,1,Transceiver);
		return 0x04;													// Will change later when checking connections
	}
	
	
	// Ack from Humpro after command is sent
	if (msgType == 0x06){
		uint8_t regNum		= (uint8_t) getChar(Transceiver);
		uint8_t regValue	= (uint8_t) getChar(Transceiver);
		return regValue;
	}
	// Message from Humpro
	
}



// Updated the destination DSN on the HUMPRO
void writeDestDSN(uint8_t destDSN[]){
	
	uint8_t destDSNCmd[] = {0xFF,0x02,0x67,0xFF};
	PORTD &= ~(1 << PORTD5);	// cmd mode
	
	for (uint8_t i = 0; i < 4; i++ ){
		
		destDSNCmd[3] = destDSN[i];
		destDSNCmd[2] = destDSNCmd[2] + 1;
		TXWrite(destDSNCmd,4,Transceiver);
		
	}
	skipBuffer();			// Clearing buffer of acknowledges
	_delay_ms(10);
	PORTD |= (1 << PORTD5);	// transmit mode
}

// Gets the DSN of the NUB that the destination phone is connected to, also returns its device number and if it was found or not (in bytes 6,7)
// after the dsn
uint8_t *getDestPhoneAdd(uint8_t destPhoneAdd[7],uint8_t *myDSN, struct networkStructure network){
	
	uint8_t phoneFound = 0;
	uint8_t *destDSN = (uint8_t*) malloc(sizeof(uint8_t)*5);
	
	
	
	// Looping through all devices in network to search for which device is connected to the desired phone
	for (uint8_t deviceNum = 0; deviceNum < network.numOfDevices; deviceNum++)
	{//1
		
		// Looping through all the connected phones of this specific NUB
		for (uint8_t phoneConnection = 0; phoneConnection < network.device[deviceNum].numOfPhoneCon; phoneConnection++)
		{//2
			
			// Comparing the destination MAC address to ones stored in network, if found assign device DSN to destination DSN
			for (uint8_t i = 0; i < 6 ; i++)
			{//3
				if (destPhoneAdd[i] != network.device[deviceNum].phoneConnections[phoneConnection][i]){
					break;
				}
				if (i == 5){
					phoneFound = 1;
					
					destDSN[5] = deviceNum;			// To return with dsn address for easier lookup
					for (uint8_t j = 0; j < 4; j++){
						destDSN[j] = network.device[deviceNum].deviceDSN[j];
					}
					destDSN[6] = 0x01;
					return destDSN;
				}
			}//3	
		}//2		
	}//1
	
	if(phoneFound != 1){
		destDSN[6] = 0xFF;
		return destDSN;
	}
}


void buildMsgPath(uint8_t destDeviceNum, uint8_t destDSN[4], uint8_t * myDSN, struct networkStructure network){
	// destDeviceNum: Number of the device in the network 
	uint8_t msgPath[5][4];
	
	
	/*
	struct localConnections{
		uint8_t deviceDSN[4];
		uint8_t numOfNubCon;
		uint8_t numOfPhoneCon;			// How many devices are connected to this one
		uint8_t nubConnections[3][4];	// The DSN addresses of the connected devices
		uint8_t phoneConnections[3][6];	// The mac addresses of connected phones
	};


	// Overall network structure, 175 Bytes
	struct networkStructure{
		uint8_t numOfDevices;
		
		struct localConnections device[5];		// Max network size of 5 connections
		
	};
	*/
	uint8_t tempMsgPath[5];		// With device network number instead of DSN
	uint8_t pathSize = 1;
	uint8_t pathFound = 0;
	uint8_t currentNodeCon[3] = {0xFF,0xFF,0xFF};		// To know which nubs are connected to the Nub currently being looked at
	uint8_t index = 0;

	uint8_t srcDeviceNum;
	
	// Getting source device number in network
	for(uint8_t i = 0; i < network.numOfDevices; i++){
		
		for(uint8_t j = 0; j < 4; j++){
			if (network.device[i].deviceDSN[j] != myDSN[j]){
				break;
			}
			if (i == 3){
				srcDeviceNum = i;
			}
		}
	}
	
	
	
	
	// Search destination device for direct connection to source first
	for(uint8_t conNum = 0; conNum < network.device[srcDeviceNum].numOfNubCon; conNum ++){
		
		
		for (uint8_t i = 0; i < 4; i ++){	//Looping through connected nub DSN
			
			if (destDSN[i] != myDSN[i]){
				break;
			}
			if(i >= 3){
				pathFound = 1;
				for (int j = 0; j< 4; j++){
					msgPath[0][j] = myDSN[j];
				}
				for (int j = 0; j< 4; j++){
					msgPath[1][j] = destDSN[j];
				}
				
			}
		}
	}
	
	
	if (pathFound == 0){
		
		// Indexing all connection device numbers
		for(uint8_t conNum = 0; conNum < network.device[srcDeviceNum].numOfNubCon; conNum++){
			for(uint8_t deviceNum = 0; deviceNum < network.numOfDevices; deviceNum++){
				for(uint8_t i = 0 ; i < 4; i++){
					if(network.device[srcDeviceNum].nubConnections[conNum][i] != network.device[deviceNum].deviceDSN[i]){
						break;
					}
					if (i > 3){
						currentNodeCon[index] = deviceNum;
						index++;
					}
				}
			}
		}
		
		for(uint8_t srcconNum = 0; srcconNum < network.device[srcDeviceNum].numOfNubCon; srcconNum++){
			
			for(uint8_t conNum = 0; conNum < network.device[currentNodeCon[srcconNum]].numOfNubCon; conNum++){
				for( uint8_t i = 0; i < 4; i++){
					if(network.device[currentNodeCon[srcconNum]].nubConnections[conNum][i] != network.device[srcDeviceNum].deviceDSN[i]){
						break;
					}
					if (i > 3){
						
					}
				}
			}
			
		}
			
	}
	
}



// Sending a simple message
void sendMessageSimple(uint8_t msgPathSize, uint8_t deviceNumInPath, uint8_t msgPath[],
						uint8_t destPhoneAdd[6],uint8_t srcPhoneAdd[6], unsigned char msg[],uint8_t msgSize){
	
	
	uint8_t msgType = 0x01;	// message
	uint8_t msgHeader[] = {msgType,deviceNumInPath,msgPathSize,msgSize};
	
	writeDestDSN(msgPath[deviceNumInPath + 1]);		// The next devices DSN
	
	// Send [msgType (1 byte), deviceNumInPath (1 byte), msgPathSize (1 byte), msgSize (1 byte), 
	//       msgPath (var), destination phone address (6 bytes), source phone address (6 bytes), message (var)]
	
	TXWrite(msgHeader,sizeof(msgHeader),Transceiver);
	TXWrite(msgPath,msgPathSize*4,Transceiver);
	TXWrite(destPhoneAdd,6,Transceiver);			// mac address
	TXWrite(srcPhoneAdd,6,Transceiver);			// mac address
	
	TXWrite(msg, msgSize,Transceiver);
	
}

// Sends an acknowledge through HUMPRO back to NUB sender
void sendAck(uint8_t srcDSN[], uint8_t destDSN[]){
	
	uint8_t msgType = 0x04;			//Acknowledge
	uint8_t msgHeader[] = {msgType,srcDSN[0],srcDSN[1],srcDSN[2],srcDSN[3]};
	
	uint8_t msg[] = {0x01};
	writeDestDSN(destDSN);
	
	TXWrite(msgHeader,5,Transceiver);
	TXWrite(msg,1,Transceiver);
	
}


// Initial broadcast to connect to other devices
void broadcastCon(uint8_t txPWR,uint8_t srcDSN[], uint8_t *networkPtr, uint16_t networkPtrSize){
	PORTD &= ~(1 << CMD);	// cmd mode
	uint8_t TXPower[4]	= {0xFF,0x02,0x02,txPWR};
	TXWrite(TXPower,4,Transceiver);
	getChar(Transceiver);
	
	
	uint8_t destDSNCMD[] = {0xFF,0x03,0x68,0xFE,0x7F};		// Putting broadcast address
	
	uint8_t msgType = 0x03;			// Connection broadcast
	uint8_t msgHeader[] = {msgType,srcDSN[0],srcDSN[1],srcDSN[2],srcDSN[3]};
	
	
	for (uint8_t i = 0; i < 4; i++){
		TXWrite(destDSNCMD,5,Transceiver);
		getChar(Transceiver);
		destDSNCMD[2] = destDSNCMD[2] + 0x01;
	}
	
	_delay_ms(10);
	PORTD |= (1 << CMD);		// Transmission mode
	_delay_ms(10);
	
	TXWrite(msgHeader,5,Transceiver);
	TXWrite(networkPtr,networkPtrSize,Transceiver);	 // Sending connected devices structure
	
	
}


// Compares the received network information from another NUB to local information
void compareNetworks(struct networkStructure *network){
	/*
	struct localConnections{
		uint8_t deviceDSN[4];	
		uint8_t numOfNubCon;
		uint8_t numOfPhoneCon;			// How many devices are connected to this one
		uint8_t nubConnections[3][4];	// The DSN addresses of the connected devices
		uint8_t phoneConnections[3][6];	// The mac addresses of connected phones
	};


	// Overall network structure, 175 Bytes
	struct networkStructure{
		uint8_t numOfDevices;
		
		struct localConnections device[5];		// Max network size of 5 connections
		
	};
	
	// How the data is formatted in the receive0 buffer
	[numOfDevices (1),																					   (1  byte)
	device[0].deviceDSN (4), device[0].numofNubCon (1), device[0].numOfPhoneCon (1),					   (6  bytes)
	device[0].nubCon[0] (4), device[0].nubCon[1] (4), device[0].nubCon[2] (4),                             (12 bytes)
	device[0].phoneCon[0] (6), device[0].phoneCon[0] (6), device[0].phoneCon[0] (6),					   (18 bytes)
	....
	]
	*/
	
	uint16_t devicePtr[MaxNetworkSize];			// "points" to rx0 buffer location of device
	uint8_t numOfDevicesInRx = getChar(Transceiver);
	
	// Setting a pointer to each device in the network structure currently stored in the rx buffer
	for (uint16_t deviceNum = 0; deviceNum < numOfDevicesInRx; deviceNum ++){
		devicePtr = parseBufferForPtr(deviceNum * (6 + 4 * MAX_NUB_CON + 6 * MAX_PHONE_CON));	
	}
	
	for (uint8_t deviceNum = 0; deviceNum < numOfDevicesInRx; deviceNum++){
		
		
	}
	
	
	
}

void updateNetwork(struct networkStructure *network){
	
}

/*
// Sending a user message in up to 256 byte packets at a time
void sendMessage(uint8_t frameType,uint8_t hopID,uint8_t seq, uint8_t destDSN[],uint8_t sourceDSN[],unsigned char msg[], uint16_t size){
	
	
	uint8_t msgType = 0x01;			// Text message
	uint8_t partialMsg[256];
	uint8_t msgWritePos = 0;
	uint8_t partialMsgSize = 0;
	uint8_t dataLength1 = 0x04 + partialMsgSize;
	uint8_t sendMsg[] = {tag1,headerLength,frameType,hopID,seq,destDSN[3],destDSN[2],destDSN[1],destDSN[0],sourceDSN[3],sourceDSN[2],sourceDSN[1],sourceDSN[0],dataLength1,
	tag2,partialMsgSize,msgType,partialMsg};
	PORTC |= (1 << PORTC2);			// Ensuring that the HUMPRO is in transmit mode
	
	while(size > 0){
		partialMsgSize = 0;
		for (uint8_t i = 0; i < 253; i++){			// Up to 253 bytes
			if (size <= 0){							// No more data to send
				break;
			}
			else{
				partialMsg[i] = msg[msgWritePos];	// Writing a partial message to be sent up to 256 bytes at a time
				msgWritePos++;
				size--;
				partialMsgSize++;
			}
		}
		
		//sendMsg = {tag1,headerLength,frameType,hopID,seq,destDSN[3],destDSN[2],destDSN[1],destDSN[0],sourceDSN[3],sourceDSN[2],sourceDSN[1],sourceDSN[0],dataLength1,
		//tag2,partialMsgSize + 1,msgType,partialMsg};
		TX0Write(msg,partialMsgSize+0x0C+5,0);		// Writing to the UART buffer, size of header plus the message

	}
	
}
*/
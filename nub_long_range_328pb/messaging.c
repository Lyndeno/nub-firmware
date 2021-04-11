#include <avr/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "handleUart.h"

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
#define DEVICESIZE 6 + MAX_NUB_CON*4 + MAX_PHONE_CON*6

extern myDSN;
extern unread0Bytes;

// The specific device connections, 35 Bytes
struct localConnections{
	uint8_t deviceDSN[4];
	uint8_t numOfNubCon;
	uint8_t numOfPhoneCon;				// How many devices are connected to this one
	uint8_t nubConnections[MAX_NUB_CON][4];	// The DSN addresses of the connected devices
	uint8_t phoneConnections[MAX_PHONE_CON][6];	// The mac addresses of connected phones
};



// Overall network structure, 175 Bytes
struct networkStructure{
	
	uint8_t numOfDevices;
	struct localConnections device[MaxNetworkSize];		// Max network size of 5 connections
	
};

struct myConData{
	uint8_t myDSN[4];
	uint8_t myNumOfNubCon;
	uint8_t myNumOfPhoneCon;
	uint8_t myNubConnections[MAX_NUB_CON][4];
	uint8_t myPhoneConnections[MAX_PHONE_CON][6];
};

			  



uint8_t handleMessages(uint8_t *myDSN, struct networkStructure *networkPtr, uint16_t sizeOfNetwork, uint8_t *networkPtr2, struct myConData* myCons){
	
	uint8_t value;
	uint8_t msgType = (uint8_t) getChar(0);		// Message type is stored as the first byte
	
	uint8_t sourceDSN[4];
	// Message that HUMPRO received over air
	
	
	if (msgType == 0x01){
		// [msgType (1 byte), deviceNumInPath (1 byte), msgPathSize (1 byte), msgSize (1 byte), 
		//  msgPath (var), destination phone address (6 bytes), source phone address (6 bytes), message (var)]
		
		uint8_t deviceNumInPath = getChar(0) + 1;		// Increment to this device
		uint8_t msgPathSize = getChar(0);
		uint8_t msgSize = getChar(0);
		uint8_t msg[msgSize];
		uint8_t msgPath[msgPathSize*4];
		for (uint8_t i = 0; i < msgPathSize*4; i ++){
			msgPath[i] = getChar(0);
		}
		
		uint8_t destPhoneAdd[6];
		for (uint8_t i = 0; i < 6; i ++){
			destPhoneAdd[i] = getChar(0);
		}
	
		uint8_t srcPhoneAdd[6];
		for (uint8_t i = 0; i < 6; i ++){
			srcPhoneAdd[i] = getChar(0);
		}
		for (uint8_t i = 0; i < msgSize; i++){
			msg[i] = getChar(0);
		}
		
		// Seeing if the this device is the last in the path, if so send message data to esp to be transmitted to the phone
		if (deviceNumInPath == msgPathSize){
			uint8_t espMsg[] = {msgType,destPhoneAdd,srcPhoneAdd,msgSize,msg};
			TXWrite(espMsg,sizeof(espMsg),1);
		}
		
		// If not pass along the message to the next NUB device
		else{
			uint8_t msgHeader[] = {msgType,deviceNumInPath,msgPathSize,msgSize};
	
			writeDestDSN(msgPath[deviceNumInPath + 1]);		// The next devices DSN
	
			// Send [msgType (1 byte), deviceNumInPath (1 byte), msgPathSize (1 byte), msgSize (1 byte), 
			//       msgPath (var), destination phone address (6 bytes), source phone address (6 bytes), message (var)]
	
			TXWrite(msgHeader,sizeof(msgHeader),0);
			TXWrite(msgPath,msgPathSize*4,0);
			TXWrite(destPhoneAdd,6,0);			// mac address
			TXWrite(srcPhoneAdd,6,0);			// mac address
	
			TXWrite(msg, msgSize,0);
		
			sendAck(myDSN,sourceDSN);
			}
	}
	if (msgType == 0x03){												// Network adjustment 
		
		sourceDSN[0] = (uint8_t) getChar(0);
		sourceDSN[1] = (uint8_t) getChar(0);
		sourceDSN[2] = (uint8_t) getChar(0);
		sourceDSN[3] = (uint8_t) getChar(0);
		uint16_t networkSize = getChar(0);
		uint8_t newCon = getChar(0);
		// Received message format: {msgType,myDSN[0],myDSN[1],myDSN[2],myDSN[3],networkSize,newCon,networkPtr};
		
		uint8_t sendToNeighbors = 1;
		
		updateNetworks(networkPtr,sizeOfNetwork,networkPtr2,myCons,newCon);
		

		// If network has been updated send to connected devices
		if (sendToNeighbors == 0){
			TXWrite("AAAAAAAAAAA",10,0);
			TXWrite(networkPtr,sizeOfNetwork,0);
			for (uint8_t deviceNum = 0; deviceNum < myCons->myNumOfNubCon; deviceNum ++){
				writeDestDSN(myCons->myNubConnections[deviceNum]);
				TXWrite(networkPtr,sizeOfNetwork,0);
			}
			return 1;
		}
		else{
			return 0;
		}
		
	}
	
	// Acknowledgment received
	if(msgType == 0x04){
		TXWrite(0x16,1,0);
		return 0x04;													// Will change later when checking connections
	}
	
	// Trying to connect
	if(msgType == 0x05){
		
		// Just sent myData, add to structure and send structure
		uint8_t sizeOfMessage;
		uint8_t deviceFound = 0;
		sizeOfMessage = getChar(0);
		sourceDSN[0] = getChar(0); 
		sourceDSN[1] = getChar(0); 
		sourceDSN[2] = getChar(0); 
		sourceDSN[3] = getChar(0); 
		
		
		if ((networkPtr->numOfDevices < MaxNetworkSize) & (myCons->myNumOfNubCon < MAX_NUB_CON)){	// Making sure there is room
			for (uint8_t deviceNum = 0; deviceNum < networkPtr->numOfDevices; deviceNum++){
				for( uint8_t i = 0; i < 4; i++){
					if (sourceDSN[i] != networkPtr->device[deviceNum].deviceDSN[i]){break;}
					if (i >= 3){
						deviceFound = 1;
					}
				}
				uint8_t nubConnections[MAX_NUB_CON][4];	// The DSN addresses of the connected devices
				uint8_t phoneConnections[MAX_PHONE_CON][6];	// The mac addresses of connected phones
				// Device has been found in current network structure, update it with the new data
				if (deviceFound == 1){
					for (uint16_t j = 0; j < sizeOfMessage; j++){
						*(networkPtr2 + 1 + DEVICESIZE*deviceNum +  j*sizeof(uint8_t) + 4) = getChar(0);
					}
					
				}
			}
			// Adding device data to the next empty place in the network
			if (deviceFound == 0){
				uint8_t message[sizeOfMessage];
				
				*(networkPtr2 + 1 + DEVICESIZE * networkPtr->numOfDevices + 0) = sourceDSN[0];
				*(networkPtr2 + 1 + DEVICESIZE * networkPtr->numOfDevices + 1) = sourceDSN[1];
				*(networkPtr2 + 1 + DEVICESIZE * networkPtr->numOfDevices + 2) = sourceDSN[2];
				*(networkPtr2 + 1 + DEVICESIZE * networkPtr->numOfDevices + 3) = sourceDSN[3];
				for (uint16_t j = 0; j < sizeOfMessage; j++){
					_delay_ms(10);
					*(networkPtr2 + 1 + DEVICESIZE * networkPtr->numOfDevices + j*sizeof(uint8_t) + 4) = getChar(0);
					
				}
				// Adding this devices dsn to newly connected device connection list
				for (uint8_t j = 0; j < 4; j++){
					networkPtr->device[networkPtr->numOfDevices].nubConnections[networkPtr->device[networkPtr->numOfDevices].numOfNubCon][j] = myCons->myDSN[j];
				}
				networkPtr->numOfDevices++;
				
			}
			
			// Adding local device to connection list of new device and updating local connection list
			deviceFound = 0;
			for (uint8_t deviceNum = 0; deviceNum < networkPtr->numOfDevices; deviceNum++){
				for( uint8_t i = 0; i < 4; i++){
					if (sourceDSN[i] != networkPtr->device[deviceNum].deviceDSN[i]){break;}
					if (i >= 3){
						deviceFound = 1;
					}
				}
				if (deviceFound == 1){
					for (uint8_t i = 0; i < 4; i++){
						networkPtr->device[deviceNum].nubConnections[networkPtr->device[deviceNum].numOfNubCon][i] = myCons->myDSN[i];
						myCons->myNubConnections[myCons->myNumOfNubCon][i] = sourceDSN[i];
					}
					myCons->myNumOfNubCon ++;
					networkPtr->device[deviceNum].numOfNubCon ++;
					
					
					// In the next two loops it searches for its own dsn to add the new connection to its list in the network
					deviceFound = 0;
					for (uint8_t myDeviceNum = 0; myDeviceNum < networkPtr->numOfDevices; myDeviceNum++){
						for( uint8_t i = 0; i < 4; i++){
							if (myCons->myDSN[i] != networkPtr->device[myDeviceNum].deviceDSN[i]){break;}
							if (i >= 3){
								deviceFound = 1;
							}
						}
						
						
						if (deviceFound == 1){
							for (uint16_t j = 0; j < 4; j++){
								networkPtr->device[myDeviceNum].nubConnections[myCons->myNumOfNubCon - 1][j] = networkPtr->device[deviceNum].deviceDSN[j];
								
							}
							
							networkPtr->device[myDeviceNum].numOfNubCon ++;
							break;

						}
					}
					break;
				}
			}

			
			sendNetworkData(networkPtr,sizeOfNetwork,myCons->myDSN,sourceDSN,1);
			return 1;
		}
		else{
			return 0;
		}
	}
	
	
	// Ack from Humpro after command is sent
	if (msgType == 0x06){
		uint8_t regNum		= (uint8_t) getChar(0);
		uint8_t regValue	= (uint8_t) getChar(0);
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
		TXWrite(destDSNCmd,4,0);
		
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
	
	TXWrite(msgHeader,sizeof(msgHeader),0);
	TXWrite(msgPath,msgPathSize*4,0);
	TXWrite(destPhoneAdd,6,0);			// mac address
	TXWrite(srcPhoneAdd,6,0);			// mac address
	
	TXWrite(msg, msgSize,0);
	
}

void sendNetworkData(uint8_t *networkPtr, uint16_t networkSize, uint8_t myDSN[], uint8_t destDSN[],uint8_t newCon){
	uint8_t msgType = 0x03;
	uint8_t msgHeader[] = {msgType,myDSN[0],myDSN[1],myDSN[2],myDSN[3],networkSize,newCon};
	
	writeDestDSN(destDSN);
	TXWrite(msgHeader,7,0);
	TXWrite(networkPtr,networkSize,0);
	
}

// Sends an acknowledge through HUMPRO back to NUB sender
void sendAck(uint8_t srcDSN[], uint8_t destDSN[]){
	
	uint8_t msgType = 0x04;			//Acknowledge
	uint8_t msgHeader[] = {msgType,srcDSN[0],srcDSN[1],srcDSN[2],srcDSN[3]};
	
	uint8_t msg[] = {0x01};
	writeDestDSN(destDSN);
	
	TXWrite(msgHeader,5,0);
	TXWrite(msg,1,0);
	
}


// Initial broadcast to connect to other devices
void broadcastCon(uint8_t txPWR, uint8_t *myCons2){
	PORTD &= ~(1 << CMD);	// cmd mode
	uint8_t TXPower[4]	= {0xFF,0x02,0x4D,txPWR};
	TXWrite(TXPower,4,0);
	_delay_ms(30);
	getChar(0);
	getChar(0);
	getChar(0);
	getChar(0);
	
	uint8_t destDSNCMD[] = {0xFF,0x03,0x68,0xFE,0x7F};		// Putting broadcast address
	
	uint8_t msgType = 0x05;			// Connection broadcast
	uint8_t myDataSize = 4 + 1 + 1 + MAX_NUB_CON*4 + MAX_PHONE_CON*6;
	uint8_t msgHeader[] = {msgType,myDataSize};
	// Message format:
	// [msgType,myDatasize,mycons.DSN, mycons.myNumOfNubCon, mycons.myNumOfPhoneCon,mycons.myNubConnections[MAX_NUB_CON][4]
	//  ,mycons.myPhoneConnections[MAX_PHONE_CON][6]
	
	
	
	for (uint8_t i = 0; i < 4; i++){
		
		TXWrite(destDSNCMD,5,0);
		getChar(0);
		destDSNCMD[2] = destDSNCMD[2] + 0x01;
		_delay_ms(10);
	}
	
	_delay_ms(50);
	PORTD |= (1 << CMD);		// Transmission mode
	_delay_ms(10);
	//while(uartReady());
	TXWrite(msgHeader,2,0);
	//while(uartReady());
	TXWrite(myCons2,myDataSize,0);	 // Sending connected devices structure
	
	
}


// Compares the received network information from another NUB to local information
void updateNetworks(struct networkStructure *networkPtr, uint16_t networkSize,uint8_t *networkPtr2,struct myConData* myCons, uint8_t newCon){
//void updateNetworks( ){
	/*
	// How the data is formatted in the receive0 buffer
	[numOfDevices (1), 																					   (1  byte)
	device[0].deviceDSN (4), device[0].numofNubCon (1), device[0].numOfPhoneCon (1),					   (6  bytes)
	device[0].nubCon[0] (4), device[0].nubCon[1] (4), device[0].nubCon[2] (4),                             (12 bytes)
	device[0].phoneCon[0] (6), device[0].phoneCon[0] (6), device[0].phoneCon[0] (6),					   (18 bytes)
	....
	]
	*/
	uint8_t networkUpdated = 0;
	uint8_t count = 0;
	uint8_t flag = 0;
	
	// Make sure all network data has been received
	while (unread0Bytes >= networkSize){
		_delay_ms(10);
		if ( count > 100 ){
			break;
		}
		count ++;
	}
	// Copying all network data received 
	for (uint8_t i = 0; i < networkSize; i ++){
		
		*(networkPtr2 + i*sizeof(uint8_t) ) = getChar(0);
					 
	}
	TXWrite(networkPtr2,networkSize,0);
	
	// Responding to the connection broadcast if newCon == 1
	if (newCon == 1){
		
		// Search each device in network and if its in local connection list, if it is and local device is not in its connection
		// list add the local connection to that devices nub list
		// If this device is in its list but not mine, add to local list
		
		uint8_t foundInLocalList = 0;
		uint8_t foundInNetworkList = 0;
		myCons->myNumOfNubCon = 0;
		for (uint8_t deviceNum = 0; deviceNum < networkPtr->numOfDevices; deviceNum++){
			// Searching through local list to see if device is in it
			for(uint8_t myDeviceNum = 0; myDeviceNum < myCons->myNumOfNubCon; myDeviceNum++){
				for (uint8_t i = 0; i < 4; i++){
					if (myCons->myNubConnections[myDeviceNum][i] != networkPtr->device[deviceNum].deviceDSN[i]){
						break;
					}
					// Device found in my local list
					if (i >=3){
						foundInLocalList = 1;
						
					}
				}
				// Now Searching their nub connection list to see if local nub is connected
				for(uint8_t theirDeviceNum = 0; theirDeviceNum < networkPtr->device[deviceNum].numOfNubCon; theirDeviceNum++){
					for (uint8_t j = 0; j < 4; j++){
						if (myCons->myDSN[j] != networkPtr->device[deviceNum].nubConnections[theirDeviceNum][j]){
							break;
						}
						if (j >=3){
							myCons->myNumOfNubCon++;
							foundInNetworkList = 1;
						}
					}
				}
				
				// Found in the local list that the device is connected, but overridden in network list so update network list connection
				if (foundInLocalList & !foundInNetworkList){
					networkPtr->device[deviceNum].numOfNubCon++;
					for (uint8_t i = 0; i < 4; i++){
						networkPtr->device[deviceNum].nubConnections[networkPtr->device[deviceNum].numOfNubCon][i] = myCons->myDSN[i];
					}
					
					foundInLocalList = 0;
					foundInNetworkList = 0;
					networkUpdated = 1;
					break;
				}
				// Found in network list but not local list, new device trying to connect to this one, add to local list
				if (!foundInLocalList & foundInNetworkList){
					
					for (uint8_t i = 0; i < 4; i++){
						myCons->myNubConnections[myCons->myNumOfNubCon][i] = networkPtr->device[deviceNum].deviceDSN[i];
					}
					foundInLocalList = 0;
					foundInNetworkList = 0;
					break;
				}
			}
		}
	}
	
	
	// Searching for local device dsn
	for(uint8_t deviceNum = 0; deviceNum < networkPtr->numOfDevices; deviceNum++){
		for (uint8_t i = 0; i < 4;i++ ){
			if (networkPtr->device[deviceNum].deviceDSN[i] != myCons->myDSN[i]){
				break;
			}
			if (i >= 3){
				flag = 1;	// found local dsn in list
			}
		}
		// Device found
		if(flag == 1){
				// Checks the values received from the network and checks them against the local values
				if (myCons->myNumOfNubCon != networkPtr->device[deviceNum].numOfNubCon){
					flag = 0;
				}
				if (myCons->myNumOfPhoneCon != networkPtr->device[deviceNum].numOfPhoneCon){
					flag = 0;
				}
				if (flag != 0){
					// Checking nub connections
					for (uint8_t i = 0; i < myCons->myNumOfNubCon; i ++){
						for( uint8_t j = 0; j < 4; j++){
							if(networkPtr->device[deviceNum].nubConnections[i][j] != myCons->myNubConnections[i][j]){
								flag = 0;
								break;
							}
						}
						if (flag == 0){
							break;
						}
					}
				}
				if (flag != 0){
					// Checking phone connections
					for (uint8_t i = 0; i < myCons->myNumOfPhoneCon; i ++){
						for( uint8_t j = 0; j < 6; j++){
							if(networkPtr->device[deviceNum].phoneConnections[i][j] != myCons->myPhoneConnections[i][j]){
								flag = 0;
								break;
							}
						}
						if (flag == 0){
							break;
						}
					}
				}
				
				// Data received is different than local data, so update network back with local data
				if (flag == 0){
					
					networkPtr->device[deviceNum].numOfNubCon = myCons->myNumOfNubCon;
					networkPtr->device[deviceNum].numOfPhoneCon = myCons->myNumOfPhoneCon;
					
					for (uint8_t i = 0; i < myCons->myNumOfNubCon; i ++){
						for( uint8_t j = 0; j < 4; j++){
							networkPtr->device[deviceNum].nubConnections[i][j] = myCons->myNubConnections[i][j];
						}
					}
					for (uint8_t i = 0; i < myCons->myNumOfPhoneCon; i ++){
						for( uint8_t j = 0; j < 6; j++){
							networkPtr->device[deviceNum].phoneConnections[i][j] = myCons->myPhoneConnections[i][j];
						}
					}
					networkUpdated = 1;
					return networkUpdated;		// Network has been updated
				}
				else{
					
					return networkUpdated;		// Network has not been updated
				}			
		}
	}
	
	// If device DSN is not found at all in search, add device connections to end network list if there is room
	if (flag == 0){
		if (networkPtr->numOfDevices != MaxNetworkSize){
			networkPtr->numOfDevices ++;
			networkPtr->device[networkPtr->numOfDevices].numOfNubCon = myCons->myNumOfNubCon;
			networkPtr->device[networkPtr->numOfDevices].numOfNubCon = myCons->myNumOfPhoneCon;
			
			for (uint8_t i = 0; i < myCons->myNumOfNubCon; i ++){
				for( uint8_t j = 0; j < 4; j++){
					networkPtr->device[networkPtr->numOfDevices].nubConnections[i][j] = myCons->myNubConnections[i][j];
				}
			}
			for (uint8_t i = 0; i < myCons->myNumOfPhoneCon; i ++){
				for( uint8_t j = 0; j < 6; j++){
					networkPtr->device[networkPtr->numOfDevices].phoneConnections[i][j] = myCons->myPhoneConnections[i][j];
				}
			}
		}
		networkUpdated = 1;
		return networkUpdated;	// Network has been updated
	}
	return networkUpdated;	// Network has been updated
}
	

// Searching through the network for which device matches the dsn it is searching for
int findDeviceNum(uint8_t DSN[], struct networkStructure* networkPtr){
	
	uint8_t deviceFound = 0;
	for (uint8_t DeviceNum = 0; DeviceNum < networkPtr->numOfDevices; DeviceNum++){
		for( uint8_t i = 0; i < 4; i++){
			if (DSN[i] != networkPtr->device[DeviceNum].deviceDSN[i]){break;}
			if (i >= 3){
				deviceFound = 1;
			}
		}
		
		
		if (deviceFound == 1){
			return DeviceNum;
		}
	}
	// Device not found 
	return -1;
	
}
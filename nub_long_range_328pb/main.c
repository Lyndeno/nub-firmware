/*
 * nub_longrange.c
 *
 * Created: 1/23/2021 3:52:34 PM
 * Author : Kyle Collins
 * Last update: 2021/3/2
 * V1: Added functionality for testing UART coms as a echo
 * Notes:
 
 */ 

#define F_CPU 1000000UL


#include <avr/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "setup.h"
#include "messaging.h"
#include "handleUart.h"

#define RSTHUM		PORTB6
#define MODEIND		PORTB7

#define CRESP		PORTD2
#define PB			PORTD3
#define CMD			PORTD5
#define SLEEPMODE	PORTD7

#define LED			PORTE1


#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1


extern rx0Buffer;

#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256

#define MaxNetworkSize 5
#define MAX_NUB_CON 3
#define MAX_PHONE_CON 3

//uint8_t connectedDevices[] = {0x0A, 0x00, 0x12, 0xEF};



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



int main(void)
{
	
	DDRE |= (1 << DDE1);
	PORTE |= (1 << LED);
	USART_setup(MYUBRR);
	sei();		// Enabling interrupts 
	
	
	_delay_ms(100);
	uint8_t *myDSN;
	struct networkStructure *networkPtr, network;
	networkPtr = &network;							// Pointer to network, to be able to transmit through UART
	uint8_t *networkPtr2 = &network;
	struct myConData *myConsptr, myCons;
	myConsptr = &myCons;
	uint16_t networkSize = sizeof(network);
	
	myDSN = setupTransceiver();
	
	
	
	uint8_t value = 0x00;
	for (uint8_t i = 0; i < sizeof(network); i++){
		*(networkPtr2 + i*sizeof(uint8_t))  = value;
	}
	// Mostly test stuff ////////////////////////////////////////////////
	for(int i = 0; i < 4; i++){
		myCons.myDSN[i] = myDSN[i];
		network.device[0].deviceDSN[i] = myDSN[i];
		//network.device[1].deviceDSN[i] = 0x66;
		//network.device[2].deviceDSN[i] = 0x77;
		
		//myCons.myNubConnections[myCons.myNumOfNubCon][i] = network.device[1].deviceDSN[i];
		//network.device[0].nubConnections[0][i] = network.device[1].deviceDSN[i];
		
		//network.device[1].nubConnections[0][i] = network.device[0].deviceDSN[i];	// Connected to this one
		//network.device[1].nubConnections[1][i] = network.device[2].deviceDSN[i];	// Connected to the other one
		
		//network.device[2].nubConnections[0][i] = network.device[1].deviceDSN[i];
		
	}
	
	for(int i = 0; i <6; i++){
		myCons.myPhoneConnections[0][i] = 0x11;	// Just giving it a default value, 
		network.device[0].phoneConnections[0][i] = myCons.myPhoneConnections[0][i];
		//network.device[1].phoneConnections[0][i] = 0x33;
		//network.device[2].phoneConnections[0][i] = 0x44;
	}
	
	
	myConsptr->myNumOfNubCon = 0;
	myCons.myNumOfPhoneCon = 1;
	network.numOfDevices = 1;
	network.device[0].numOfNubCon = myCons.myNumOfNubCon;	
	network.device[0].numOfPhoneCon = myCons.myNumOfPhoneCon;	
	
	//network.device[1].numOfNubCon = 2;
	//network.device[2].numOfNubCon = 1;
	//network.device[0].numOfPhoneCon = 1;
	//network.device[1].numOfPhoneCon = 1;
	//network.device[2].numOfPhoneCon = 1;
	uint8_t output = 0;
	//////////////////////////////////////////////////////////////////
	
	uint8_t firstCon = 1;
	skipBuffer(0);
	
	uint8_t i = 0;
	uint8_t UARTPort;
    while (1){		
		
		if (firstCon == 1){
			
			broadcastCon(0x00,myConsptr);
			
			_delay_ms(7500);
		}
		
		
		if(Bytes0UnRead() > 0){
			UARTPort = 0;
			
			output = handleMessages(UARTPort,networkPtr,networkSize,networkPtr2,myConsptr);
			if (output == 3 || output == 5){
				firstCon = 0;
			}
			
			skipBuffer(0);
		}
		if(Bytes1UnRead() > 0){
			UARTPort = 1;
			handleMessages(UARTPort,networkPtr,networkSize,networkPtr2,myConsptr);
			skipBuffer(1);
			TXWrite(networkPtr,networkSize,1);
		}
		
		
		//checkReg();
		//test();
		/*
		i++;
		if (i > 100){
			
			TXWrite(networkPtr,sizeof(network),1);
			i = 0;
		}
		*/
		_delay_ms(100);
		
    }
}


void checkReg(){
	
	uint8_t hopTable[4]	= {0xFF,0x02,0x00,0x00};
	uint8_t TXPower[4]	= {0xFF,0x02,0x4D,0x01};		// Setting the power to 25dBm for initial search/ default 
		
	uint8_t UARTBaud[4]	= {0xFF,0x02,0x03,0x02};		// Setting BAUD rate to 19.2 kb/s ( also means this is the transmission speed)
	uint8_t addMode[4]	= {0xFF,0x02,0x4F,0x04};		// Setting address mode as DSN (Volatile address)
	uint8_t showVer[4]	= {0xFF,0x02,0x0A,0x00};		// Do not show version at start up
	uint8_t CMDHold[4]	= {0xFF,0x02,0x6E,0x01};		// If CMD_BAR is low, wait until it goes high to send to mcu
	uint8_t compat[4]	= {0xFF,0x02,0x70,0x00};		// Compatibility mode to enable broadcast function for DSN
	uint8_t autoAdd[4]	= {0xFF,0x02,0x71,0x04};		// Auto addressing for DNS mode
	uint8_t myDSN3[4]	= {0xFF,0x02,0xFE,0x34};				// Read command for preprogrammed DNS values, DSN3 is MSB
	uint8_t myDSN2[4]	= {0xFF,0x02,0xFE,0x35};
	uint8_t myDSN1[4]	= {0xFF,0x02,0xFE,0x36};
	uint8_t myDSN0[4]	= {0xFF,0x02,0xFE,0x37};
	uint8_t LStatus[5]	= {0xFF,0x03,0xFE,0xFE,0x46};	// Status of output lines
	
	PORTD &= ~(1 << PORTD5);	// cmd mode
	
	uint8_t cmds[][4] = {{0xFF,0x02,0xFE,0x4F},		//ADDMODE			0
						{0xFF,0x02,0xFE,0x6B},		//DESNTDSN0			1
						{0xFF,0x02,0xFE,0x70},		//COMPAT			2
						{0xFF,0x02,0xFE,0x4D},		//TX Power			3
						{0xFF,0x02,0xFE,0x4E},		//Baud Rate			4
						{0xFF,0x02,0xFE,0x6E}
						};	
	
	TXWrite(cmds[0],4,0);	
	TXWrite(cmds[1],4,0);
	TXWrite(cmds[2],4,0);					
	TXWrite(cmds[3],4,0);		
	TXWrite(cmds[4],4,0);
	_delay_ms(50);
	
	
	
	
	PORTD |= ~(1 << PORTD5);	// cmd mode
	
}

void test(uint8_t srcDSN){
	uint8_t Add1[] = {0x0A,0x00,0x12,0xEF}; // Left 
	uint8_t Add2[] = {0X0A,0X00,0X12,0XF0}; // Right
	writeDestDSN(Add1);
	
	sendMessageSimple2(Add1,Add2,"HHHHHHHHHH\n",11);
	
}



void sendMessageSimple2(uint8_t srcPhoneAdd[4],uint8_t destPhoneAdd[4], unsigned char msg[],uint8_t msgSize){
	
	
	uint8_t msgType = 0x01;	// message
	
	
	
	
	// Send [msgType (1 byte), deviceNumInPath (1 byte), msgPathSize (1 byte), msgSize (1 byte),
	//       msgPath (var), destination phone address (6 bytes), source phone address (6 bytes), message (var)]
	
	
	writeDestDSN(destPhoneAdd);
	_delay_ms(50);
	
	TXWrite(msg,msgSize,0);			// mac address
	_delay_ms(50);
	
	
	
}



// (struct networkStructure*,uint8_t, uint16_t, struct myConData*,uint8_t
/*
handleMessages(myCons.myDSN,networkPtr,sizeof(network),networkPtr2,myConsptr);
uint8_t updateNetworks(struct networkStructure *network, uint8_t *networkPtr, uint16_t networkSize,struct myConData* myCons, uint8_t newCon)
uint8_t handleMessages(uint8_t *myDSN, struct networkStructure* network, uint16_t sizeOfNetwork, uint8_t *networkPtr, struct myConData* myCons);
uint8_t handleMessages(uint8_t *myDSN, struct networkStructure* network, uint16_t sizeOfNetwork, uint8_t *networkPtr, struct myConData* myCons){
sendToNeighbors = updateNetworks(network,networkPtr,sizeOfNetwork,myCons,newCon);
*/
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


#define RSTHUM		PORTB6
#define MODEIND		PORTB7

#define CRESP		PORTD2
#define PB			PORTD3
#define CMD			PORTD5
#define SLEEPMODE	PORTD7

#define LED			PORTE1


#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1

extern unreadBytes;
extern rx0Buffer;
extern rx0WritePos;
#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256

#define MaxNetworkSize 5
#define MAX_NUB_CON 3
#define MAX_PHONE_CON 3

uint8_t connectedDevices[] = {0x0A, 0x00, 0x12, 0xEF};
uint8_t *myDSN;


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
	
	struct localConnections device[5];		// Max network size of 5 connections 
	
};



int main(void)
{
	
	DDRE |= (1 << DDE1);
	PORTE |= (1 << LED);
	USART_setup(MYUBRR);
	sei();		// Enabling interrupts 
	
	
	_delay_ms(100);
	myDSN = setupTransceiver();
	struct networkStructure *networkptr, network;
	networkptr = &network;							// Pointer to network, to be able to transmit through UART
	
	
	
    while (1) 
		
    {	
		//TXWrite(var,6,0);
		//TXWrite(networkptr,sizeof(network),0);
		broadcastCon(0x01,myDSN,networkptr,sizeof(network));
		//broadcastCon(0x9,myDSN,networkptr);
		//sendMessageSimple(myDSN,connectedDevices,message,5);
		_delay_ms(5000);
		
    }
}



void syncNetwork(){
	
}


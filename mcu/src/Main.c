#include "io.h"

#include <stdio.h>
#include <stdlib.h>

#include "comms.h"
#include "buffer.h"
#include "routing.h"

// Kyle Stuff

#define LED			PORTE1

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

// End of Kyle stuff


// TODO: Match UART to physical implementation, UART0 and UART1

// IDEA: wifi and transceiver send/receive will probably use very similar code, 
// figure out how to pass UART registers to function

void handle_message(circular_buf * );
void check_frame (circular_buf * );

int main (void) {
    // Initialize buffers first as UART needs them
    UART_init(UBRR_WIFI);
    DDR(PORT_STATUS_LED) |= (1<<PIN_STATUS_LED);
    DDR(PORT_TEST_LED) |= (1<<PIN_TEST_LED);

    while (1) {
        PORT(PORT_STATUS_LED) ^= (1<<PIN_STATUS_LED);

        read_frame(&buff_wifi_rx);
        
    }

    /*DDRE |= (1 << DDE1);
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
		
    }*/
}

void syncNetwork(){
	
}
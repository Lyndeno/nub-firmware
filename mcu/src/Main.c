#include "io.h"

#include <stdio.h>
#include <stdlib.h>

#include "comms.h"
#include "buffer.h"
#include "routing.h"


// TODO: Match UART to physical implementation, UART0 and UART1

// IDEA: wifi and transceiver send/receive will probably use very similar code, 
// figure out how to pass UART registers to function

void handle_message(circular_buf * );
void check_frame (circular_buf * );

// Fix this

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

//

int main (void) {
    // Initialize buffers first as UART needs them


    init_buffer(&buff_wifi_rx, BUFFER_LENGTH_BYTES);
    //init_buffer(&buff_trans_rx, BUFFER_LENGTH_BYTES);
    
    
    init_device_table();

    DDRE |= (1 << DDE1);
	PORTE |= (1 << LED);
	USART_setup(MYUBRR);
    UART_WiFi_init(UBRR_WIFI);
    sei();

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

    uint8_t firstCon = 1;
	skipBuffer(0);
	
	uint8_t i = 0;
	uint8_t UARTPort;

    while (1) {
        PORT(PORT_STATUS_LED) ^= (1<<PIN_STATUS_LED);

        read_frame(&buff_wifi_rx);
        read_frame(&buff_trans_rx);

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

        _delay_ms(100);
        
    }
}
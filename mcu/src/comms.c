#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "comms.h"
#include "io.h"

#define RX0_BUFFER_SIZE 256
#define RX1_BUFFER_SIZE 256

#define RSTHUM		PORTB6
#define MODEIND		PORTB7

#define CRESP		PORTD2
#define PB			PORTD3
#define CMD			PORTD5
#define SLEEPMODE	PORTD7

#define PWRHUM		PORTE1


/*extern rx0ReadPos;
extern rx0WritePos;
extern unread0Bytes;*/


unsigned char rx0Buffer[RX0_BUFFER_SIZE];
unsigned char rx0ReadPos = 0;
unsigned char rx0WritePos = 0;
unsigned char rx0BufferPointer = 0;

unsigned char rx1Buffer[RX1_BUFFER_SIZE];
unsigned char rx1ReadPos = 0;
unsigned char rx1WritePos = 0;
unsigned char rx1BufferPointer = 0;


uint8_t unread0Bytes = 0;
uint8_t unread1Bytes = 0;
uint8_t newPacket = 0;

/*
ISR(WIFI_RX_vect) {
    // when interrupt is triggered then write UDR into buffer so we do not lose information
    buff_wifi_rx.buff[buff_wifi_rx.tail++] = UDR0;
    buff_wifi_rx.free--;

    PORT(PORT_TEST_LED) ^= (1<<PIN_TEST_LED);

    if (buff_wifi_rx.tail >= buff_wifi_rx.size) {
        buff_wifi_rx.tail = 0;
    }
}

ISR(TRANS_RX_vect) {
    // when interrupt is triggered then write UDR into buffer so we do not lose information
    buff_trans_rx.buff[buff_trans_rx.tail++] = UDR1;
    buff_trans_rx.free--;

    PORT(PORT_TEST_LED) ^= (1<<PIN_TEST_LED);

    if (buff_trans_rx.tail >= buff_trans_rx.size) {
        buff_trans_rx.tail = 0;
    }
}*/

/** 
 * Initializes a given buffer for use. This function sets the baud rate, enables transmission and reception,
 * and enables a receive interrupt for UART.
 */
void UART_WiFi_init (unsigned int ubrr) {
    // Disable power reduction for USARTS
    //PRR0 |= (1<<PRUSART0)|(1<<PRUSART1);

    // Enable WiFi UART
    UBRRH_WIFI = (unsigned char)(ubrr>>8); // Set high bits of UBRR for baud rate
    UBRRL_WIFI = (unsigned char)(ubrr); // Set low bits of UBRR for baud
    UCSRB_WIFI = (1<<RXEN_WIFI)|(1<<TXEN_WIFI); // Enable receiving and transmission on UART
    UCSRB_WIFI |= (1<<RXCIE_WIFI); // enable receive interrupt for circular buffer

    // Enable WiFi UART
    UBRRH_TRANS = (unsigned char)(ubrr>>8); // Set high bits of UBRR for baud rate
    UBRRL_TRANS = (unsigned char)(ubrr); // Set low bits of UBRR for baud
    UCSRB_TRANS = (1<<RXEN_TRANS)|(1<<TXEN_TRANS); // Enable receiving and transmission on UART
    UCSRB_TRANS |= (1<<RXCIE_TRANS); // enable receive interrupt for circular buffer

    sei(); //enable interrupts
}

/** 
 * Transmit byte to WiFi modules. This function waits for the ATmega to finish transmitting the last byte before
 * attempting another transfer.
 */
void UART_WiFi_TX (uint8_t byte) {
    while(!( UCSRA_WIFI & (1<<UDRE_WIFI))); // Wait for TX to finish
    UDR_WIFI = byte; // Write for TX
}

void UART_TRANS_TX (uint8_t byte) {
    while(!( UCSRA_TRANS & (1<<UDRE_TRANS))); // Wait for TX to finish
    UDR_TRANS = byte; // Write for TX
}





void delay_ms(int t){
	int intervals = t/50;
	
	for (int i = 0; i < intervals; i++){
		
		_delay_ms(10);
	}
	
}

// Setting up USART for communication between HUMPRO900 module and the MCU
void USART_setup(unsigned int ubrr){
	
	// UART 0
	UBRR0H = (unsigned char)(ubrr>>8);			// Setting baud rate
	UBRR0L = (unsigned char)ubrr;
	UCSR0A = (1<<UDRE0)|(1<<U2X0);
	UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);				// Enabling Tx & Rx
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);			// 2 stop bits & 8 bit character size
	
	// UART 1
	UBRR1H = (unsigned char)(ubrr>>8);			// Setting baud rate
	UBRR1L = (unsigned char)ubrr;
	UCSR1A = (1<<UDRE1)|(1<<U2X0);
	UCSR1B = (1<<RXEN1)|(1<<RXCIE1)|(1<<TXEN1);				// Enabling Tx & Rx
	UCSR1C = (1<<USBS1)|(3<<UCSZ00);			// 2 stop bits & 8 bit character size
	
	
	
}



// Setting up ports for HUMPRO900 transceiver
uint8_t * setupTransceiver(){
	
	sei();		// Enabling interrupts
	DDRB |= (1 << DDB7);	//PD4 PWR High for normal op, low for low power operation (power down)
	//DDRC &= ~(1 << DDC3);	//PC3 CRESP_BAR
	DDRC &= ~(1 << DDC4);	//PC4 CTS_BAR (high when busy, low when module is ready)
	DDRB |= (1 << DDB6);	//PC5 Reset, resets when low
	
	
	//DDRD &= ~(1 << DDD0);	//PD2 Buffer empty status (high when buffer is empty) input
	DDRD |= (1 << DDD3);	//PD3 "PUSH button" PULL LOW
	DDRD |= (1 << DDD5);	//PD5 CMD_BAR (high for data transmit, low for commands)
	
	PORTD &= ~(1<< PORTD3);
	PORTB &= ~(1 << PORTB6);		// Reset norm operation
	_delay_ms(100);
	PORTB |= (1 <<PORTB6);
	_delay_ms(1000);
	rx0ReadPos = rx0WritePos;	// Ignoring reset message
	
	
	PORTD &= ~(1 << PORTD5);	// Command mode
	
	
	uint8_t hopTable[4]	= {0xFF,0x02,0x00,0x00};
	uint8_t TXPower[4]	= {0xFF,0x02,0x4D,0x01};		// Setting the power to 25dBm for initial search/ default value
	uint8_t UARTBaud[4]	= {0xFF,0x02,0x03,0x02};		// Setting BAUD rate to 19.2 kb/s ( also means this is the transmission speed)
	uint8_t addMode[4]	= {0xFF,0x02,0x4F,0x04};		// Setting address mode as DSN (Volatile address)
	uint8_t showVer[4]	= {0xFF,0x02,0x0A,0x00};		// Do not show version at start up
	uint8_t CMDHold[4]	= {0xFF,0x02,0x6E,0x00};		// If CMD_BAR is low, wait until it goes high to send to mcu
	uint8_t compat[4]	= {0xFF,0x02,0x70,0x00};		// Compatibility mode to enable broadcast function for DSN
	uint8_t autoAdd[4]	= {0xFF,0x02,0x71,0x04};		// Auto addressing for DNS mode
	uint8_t myDSN3[4]	= {0xFF,0x02,0xFE,0x34};				// Read command for preprogrammed DNS values, DSN3 is MSB
	uint8_t myDSN2[4]	= {0xFF,0x02,0xFE,0x35};
	uint8_t myDSN1[4]	= {0xFF,0x02,0xFE,0x36};
	uint8_t myDSN0[4]	= {0xFF,0x02,0xFE,0x37};
	uint8_t LStatus[5]	= {0xFF,0x03,0xFE,0xFE,0x46};	// Status of output lines
	
	// Send all defined commands through uart
	//TX0Write(hopTable[0],4);
	//delay_ms(50);
	//TXWrite(TXPower,4,0);
	//delay_ms(50);
	//getChar(0);
	//TX0Write(UARTBaud[0],4);
	
	
	//TXWrite(addMode,4,0);
	//delay_ms(50);
	//getChar(0);
	
	//TX0Write(showVer[0],4);
	//TXWrite(CMDHold,4,0);
	//delay_ms(50);
	//getChar(0);
	//TXWrite(compat,4,0);
	//delay_ms(50);
	//getChar(0);
	//TX0Write(autoAdd[0],4);
	
	
	// Getting local DNS info (third char returned)
	uint8_t *myDSNVal = malloc(4*sizeof(uint8_t));
	
	TXWrite(myDSN3,4,0);
	_delay_ms(100);
	while(unread0Bytes < 3);
	
	rx0ReadPos = rx0WritePos -1;		// Ignoring other bytes received, only care about DSN value

	myDSNVal[0] = getChar(0);
	
	
	TXWrite(myDSN2,4,0);
	_delay_ms(100);
	while(unread0Bytes < 3);
		
	rx0ReadPos = rx0WritePos -1;
	myDSNVal[1] = getChar(0);
	
	
	TXWrite(myDSN1,4,0);
	_delay_ms(100);
	while(unread0Bytes < 3);
	rx0ReadPos = rx0WritePos -1;
	myDSNVal[2] = getChar(0);
	
	
	TXWrite(myDSN0,4,0);
	_delay_ms(100);
	while(unread0Bytes < 3);
	rx0ReadPos = rx0WritePos -1;
	myDSNVal[3] = getChar(0);
	
	_delay_ms(3000);
	
	TXWrite(myDSNVal,4,0);
	_delay_ms(1000);
	PORTD |= (1 << PORTD5);	// Data transmit mode
	// Resetting HUMPRO to have non volatile registers update
	//PORTC &= ~(1 << PORTC5);
	
	//PORTC |= (1 << PORTC5);
	
	return myDSNVal;
	
}






// Taking input characters of size size and sending them through the UART TX without storing the values in a buffer first
void TXWrite(unsigned char c[], uint8_t size, uint16_t uartPort ){
	
	// Writing to the transmit port the amount of characters defined by size
	uint16_t count = 0;
	while (size > 0 ){
		if (uartPort == 0){
			
			while (!(UCSR0A & (1<<UDRE0)));
			UDR0 = c[count];
			count++;
			size --;
		}
		else{
			while (!(UCSR1A & (1<<UDRE1)));
			UDR1 = c[count];
			count++;
			size --;
		}
		
		
	}
	
}



// Looking at next char
uint8_t peekChar(uint8_t UARTPort){
	uint8_t ret = '\0';
	
	if (UARTPort == 0){
		if (rx0ReadPos != rx0WritePos){
			ret = rx0Buffer[rx0ReadPos];
		}
	}
	else{
		if (rx1ReadPos != rx1WritePos){
			ret = rx1Buffer[rx1ReadPos];
		}
	}
	
	
	return ret;
}


// Getting each unsigned character
unsigned char getChar(uint8_t UARTPort){
	unsigned char ret = '\0';
	
	if ( UARTPort == 0){
		
		if (rx0ReadPos != rx0WritePos){
			unread0Bytes --;
			ret = rx0Buffer[rx0ReadPos];
			rx0ReadPos ++;
			
			if (rx0ReadPos >= RX0_BUFFER_SIZE){
				rx0ReadPos = 0;
			}
		}
	}
	
	else{
		
		if (rx1ReadPos != rx1WritePos){
			unread1Bytes --;
			ret = rx1Buffer[rx1ReadPos];
			rx1ReadPos ++;
			
			if (rx0ReadPos >= RX1_BUFFER_SIZE){
				rx1ReadPos = 0;
			}
		}
	}
	
	return ret;
}

// Skipping to the most current point in the buffer
void skipBuffer(uint8_t UARTPort){
	
	if(UARTPort == 0){
		unread0Bytes = 0;
		rx0ReadPos = rx0WritePos;
	}
	else{
		unread1Bytes = 0;
		rx1ReadPos = rx1WritePos;
	}
	
}

// Allows the program to jump through the rx0 buffer 
/*uint8_t parseBufferForVal(int movePtr, uint16_t jmpToPos){
	
	uint8_t returnChar;
	// No jump to specific address, 
	if (jmpToPos == 0){
		if (rx0ReadPos + movePtr > RX0_BUFFER_SIZE){
			rx0ReadPos = rx0ReadPos + movePtr - RX0_BUFFER_SIZE;
		}
		else{
			rx0ReadPos= rx0ReadPos + movePtr;
		}
		
		returnChar = rx0Buffer[rx0ReadPos];
		if (movePtr > 0){
			unread0Bytes = unread0Bytes - movePtr;
		}
		
	}
	
	else{
		
		// Jump to specific address then move read position 
		if (jmpToPos < rx0ReadPos){
			unread0Bytes = RX0_BUFFER_SIZE - rx0ReadPos + jmpToPos;
		}
		else{
			unread0Bytes = jmpToPos - rx0ReadPos;
		}
		
		rx0ReadPos = jmpToPos;
		
		if (rx0ReadPos + movePtr > RX0_BUFFER_SIZE){
			rx0ReadPos = rx0ReadPos + movePtr - RX0_BUFFER_SIZE;
		}
		else{
			rx0ReadPos = rx0ReadPos + movePtr;
		}
		
		
		returnChar = rx0Buffer[rx0ReadPos];
		
		if (movePtr > 0){
			unread0Bytes = unread0Bytes - movePtr;
		}
	}
	
	return returnChar;
	
}*/

/*
uint16_t  parseBufferForPtr(int movePtr){
	
	if (rx0ReadPos + movePtr > RX0_BUFFER_SIZE){
		return rx0ReadPos + movePtr - RX0_BUFFER_SIZE;
	}
	else{
		return rx0ReadPos + movePtr;
	}
	
}*/

uint8_t Bytes0UnRead(){
	
	return unread0Bytes;
}
uint8_t Bytes1UnRead(){
	return unread1Bytes;
}

uint8_t uartReady(){
	if((PINC & (1 << PINC4)) == (1 << PINC4)){
		return 1;
	}
	else{
		return 0;
	}
}




// Interrupt for receiving UART data from RX 0
ISR(USART0_RX_vect){
	unread0Bytes++;
	rx0Buffer[rx0WritePos] = UDR0;
	rx0WritePos++;
	
	if(rx0WritePos >= RX0_BUFFER_SIZE)
	{
		rx0WritePos = 0;
	}
	
	
}

ISR(USART1_RX_vect){
	unread1Bytes++;
	rx1Buffer[rx1WritePos] = UDR1;
	rx1WritePos++;
	
	if(rx1WritePos >= RX1_BUFFER_SIZE)
	{
		rx1WritePos = 0;
	}
}




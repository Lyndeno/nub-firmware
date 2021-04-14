#define F_CPU 1000000UL

#include <avr/delay.h>
#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define RSTHUM		PORTB6
#define MODEIND		PORTB7

#define CRESP		PORTD2
#define PB			PORTD3
#define CMD			PORTD5
#define SLEEPMODE	PORTD7

#define PWRHUM		PORTE1


extern rx0ReadPos;
extern rx0WritePos;
extern unread0Bytes;



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
	
	
	
	TXWrite(myDSNVal,4,0);
	_delay_ms(1000);
	PORTD |= (1 << PORTD5);	// Data transmit mode
	// Resetting HUMPRO to have non volatile registers update
	//PORTC &= ~(1 << PORTC5);
	
	//PORTC |= (1 << PORTC5);
	
	return myDSNVal;
	
}

/*
* Created: 2021/01/23
* Author : Kyle Collins
* Used to set up LINX HUMPRO 900 transceiver through atmega328pb uart
*/

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>




// Setting up USART for communication between HUMPRO900 module and the MCU
void USART_setup(unsigned int ubrr){
	
	UBRR0H = (uint8_t)(ubrr>>8);						// Setting baud rate
	UBRR0L = (uint8_t)ubrr;
	UCSR0A = (1<<UDRE0)|(1<<U2X0);
	UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);				// Enabling Tx & Rx
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);						// 2 stop bits & 8 bit character size
	
}


// Setting up ports for HUMPRO900 transceiver
uint8_t * setupTransceiver(){
	
	DDRC |= (1 << DDC2);	//PC2 CMD_BAR (high for data transmit, low for commands)
	DDRC &= ~(1 << DDC3);	//PC3 CRESP_BAR
	DDRC &= ~(1 << DDC4);	//PC4 CTS_BAR (high when busy, low when module is ready)
	DDRC |= (1 << DDC5);	//PC5 Reset, resets when low
	
	DDRD &= ~(1 << DDD0);	//PD0 CMD_DATA_out
	DDRD |= (1 << DDD1);	//PD1 CMD_DATA_IN
	DDRD &= ~(1 << DDD2);	//PD2 Buffer empty status (high when buffer is empty) input
	DDRD &= ~(1 << DDD3);	//PD3 Exception status
	DDRD |= (1 << DDD4);	//PD4 PWR High for normal op, low for low power operation (power down)
	
	
	PORTC |= (1 << PORTC5);		// Reset norm operation
	PORTD |= (1 << PORTD4);		// PWR, norm operation
	
	PORTC &= ~(1 << PORTC2);	// Command mode
	
	uint8_t hopTable[4]	= {0xFF,0x02,0x00,0x00};
	uint8_t TXPower[4]	= {0xFF,0x02,0x02,0x0A};		// Setting the power to 25dBm for initial search/ default value
	uint8_t UARTBaud[4]	= {0xFF,0x02,0x03,0x02};		// Setting BAUD rate to 19.2 kb/s ( also means this is the transmission speed)
	uint8_t addMode[4]	= {0xFF,0x02,0x04,0x04};		// Setting address mode as DSN
	uint8_t showVer[4]	= {0xFF,0x02,0x0A,0x00};		// Do not show version at start up
	uint8_t CMDHold[4]	= {0xFF,0x02,0x23,0x00};		// Outputs received RF data right away, makes CRESP high
	uint8_t compat[4]		= {0xFF,0x02,0x25,0x00};		// Compatibility mode to enable broadcast function for DSN
	uint8_t autoAdd[4]	= {0xFF,0x02,0x26,0x04};		// Auto addressing for DNS mode
	uint8_t myDSN3[3]		= {0xFF,0x01,0x34};				// Read command for preprogrammed DNS values, DSN3 is MSB
	uint8_t myDSN2[3]		= {0xFF,0x01,0x35};
	uint8_t myDSN1[3]		= {0xFF,0x01,0x36};
	uint8_t myDSN0[3]		= {0xFF,0x01,0x37};
	uint8_t LStatus[5]	= {0xFF,0x03,0xFE,0xFE,0x46};	// Status of output lines
	
	// Send all defined commands through uart
	USART_Transmit(hopTable[0],4);
	USART_Transmit(TXPower[0],4);
	USART_Transmit(UARTBaud[0],4);
	USART_Transmit(addMode[0],4);
	USART_Transmit(showVer[0],4);
	USART_Transmit(CMDHold[0],4);
	USART_Transmit(compat[0],4);
	USART_Transmit(autoAdd[0],4);
	
	// Getting local DNS info
	uint8_t *myDSN = malloc(4*sizeof(char));
	USART_Transmit(myDSN3[0],3);
	myDSN[0] = USART_Receive();
	USART_Transmit(myDSN2[0],3);
	myDSN[1] = USART_Receive();
	USART_Transmit(myDSN1[0],3);
	myDSN[2] = USART_Receive();
	USART_Transmit(myDSN0[0],3);
	myDSN[3] = USART_Receive();
	
	PORTC |= (1 << PORTC2);	// Data transmit mode
	// Resetting HUMPRO to have non volatile registers update
	PORTC &= ~(1 << PORTC5);
	_delay_ms(20);
	PORTC |= (1 << PORTC5);
	
	return myDSN;
}




// Transmit variable size bits to HUMPRO900
void USART_Transmit( uint8_t *toSend, int size ) {
	uint8_t data;
	for (int i = 0; i < size; i++){
		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) );
		data = *(toSend + size*i)
		//Put data into buffer, sends the data */
		UDR0 = data;
	}
}


// Receive 8bits from HUMPRO900
uint8_t USART_Receive( void ) {
	int i = 0;
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) ){
		if (i > 10){
			return 0;
			
		}
		else{
			i++;
			_delay_ms(10);
		}
	}
	/* Get and return received data from buffer */
	return UDR0;
}



/*
 * nub_longrange.c
 *
 * Created: 1/23/2021 3:52:34 PM
 * Author : Kyle Collins
 * Last update: 2021/3/2
 * V1: Added functionality for testing UART coms as a echo
 */ 

#define F_CPU 1000000UL	

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>


#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1

#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256


unsigned char rx0Buffer[RX_BUFFER_SIZE];
unsigned char rx0ReadPos = 0;
unsigned char rx0WritePos = 0;
unsigned char rx0BufferPointer = 0;

unsigned char tx0Buffer[TX_BUFFER_SIZE];
unsigned char tx0ReadPos = 0;
unsigned char tx0WritePos = 0;

void testUART(void);
void appendTX0(unsigned char );
void TX0Write(unsigned char [], uint8_t);
unsigned char getChar(void);
void USART_setup(unsigned int);


int main(void)
{
	
	sei();		// Enabling interrupts 
	USART_setup(MYUBRR);
	unsigned char array[100];		// Test array to ensure buffer is working
	int i = 0;
    while (1) 
    {	
		if (rx0ReadPos != rx0WritePos){	
			array[i] = getChar();
			i++;
			if (i == 100){
				TX0Write(array,100);
				i = 0;
			}		
					
		}

		_delay_ms(10);
    }
}


// Setting up USART for communication between HUMPRO900 module and the MCU
void USART_setup(unsigned int ubrr){
	
	UBRR0H = (unsigned char)(ubrr>>8);			// Setting baud rate
	UBRR0L = (unsigned char)ubrr;				
	UCSR0A = (1<<UDRE0)|(1<<U2X0);
	UCSR0B = (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);				// Enabling Tx & Rx
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);			// 2 stop bits & 8 bit character size 
	
}

/*
// Writing a char to the transmit buffer to be sent out via
void appendTX0(unsigned char c){
	
	tx0Buffer[tx0WritePos] = c;
	tx0WritePos ++;
	if (tx0WritePos >= TX_BUFFER_SIZE){
		tx0WritePos = 0;
	}
}
*/

// Taking input characters of size size and sending them through the UART0 TX
void TX0Write(unsigned char c[], uint8_t size ){
	
	// Writing entered information to transmit buffer
	for ( int i = 0 ; i < size; i++){
		
		tx0Buffer[tx0WritePos] = c;
		tx0WritePos++;
		// Implementing a circular buffer
		if (tx0WritePos >= TX_BUFFER_SIZE){
			tx0WritePos = 0;
		}
	}
	
	// Writing to the transmit port the amount of characters defined by size
	while (tx0ReadPos != tx0WritePos){
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = tx0Buffer[tx0ReadPos];
		tx0ReadPos ++;
		if (tx0ReadPos >= TX_BUFFER_SIZE){
			tx0ReadPos++;
		}
	}
	
}


// Looking at next char
uint8_t peekChar(void){
	uint8_t ret = '\0';
	
	if (rx0ReadPos != rx0WritePos){
		ret = rx0Buffer[rx0ReadPos];
	}
	
	return ret;
}

// Getting each unsigned character
unsigned char getChar(void){
	unsigned char ret = '\0';
	
	if (rx0ReadPos != rx0WritePos){
		ret = rx0Buffer[rx0ReadPos];
		rx0ReadPos ++;
		
		if (rx0ReadPos >= RX_BUFFER_SIZE){
			rx0ReadPos = 0;
		}
	}
	return ret;
}


// Look at tag, have buffer pointer jump through data and get parts we care about
void getData(){
	
	uint8_t tag = peekChar();
	
	// Ack from Humpro
	if (tag == 0x06){
		//..... Add code 
	}
	// Message from Humpro
	if (tag == 0x01){
		//... Add code 
	}
}


// Interrupt for receiving UART data from RX 0 
ISR(USART_RX_vect){
	
	rx0Buffer[rx0WritePos] = UDR0;
	rx0WritePos++;
	
	if(rx0WritePos >= RX_BUFFER_SIZE)
	{
		rx0WritePos = 0;
	}
	
	
}
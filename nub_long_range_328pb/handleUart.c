

#include <avr/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define RX0_BUFFER_SIZE 256
#define RX1_BUFFER_SIZE 256


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

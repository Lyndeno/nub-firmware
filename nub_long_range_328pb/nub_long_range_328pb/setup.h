#ifndef SETUP_H
#define SETUP_H

#include <avr/delay.h>
#include <stdlib.h>
#include <stdio.h>


uint8_t * setupTransceiver();


void USART_setup(unsigned int ubrr);

void delay_ms(int t);


#endif
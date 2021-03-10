#ifndef COMMS_H
#define COMMS_H

#include <avr/interrupt.h>

// Interrupt on UART0 receive
ISR(USART0_RX_vect);

// Initialize UART
void UART_init(unsigned int);



#endif
#ifndef COMMS_H
#define COMMS_H

#include <avr/interrupt.h>

#define FRAME_LENGTH_MAX 4
#define ADDR_LENGTH 6 // MAC address is 6 bytes
#define TYPE_LENGTH 1 // one char should suffice
#define MESSAGE_LENGTH_MAX (FRAME_LENGTH_MAX - TYPE_LENGTH - ADDR_LENGTH)
#define BUFFER_LENGTH_FRAMES 4 // number of frames
#define BUFFER_LENGTH_BYTES (BUFFER_LENGTH_FRAMES * FRAME_LENGTH_MAX)

extern uint16_t buff_read;
extern uint16_t buff_write;
extern unsigned char buffer0[];

// Interrupt on UART0 receive
ISR(USART0_RX_vect);

// Initialize UART
void UART_init(unsigned int);

unsigned char read_buffer(void);
unsigned char check_buffer(void);

#endif
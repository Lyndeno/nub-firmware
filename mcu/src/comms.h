#ifndef COMMS_H
#define COMMS_H

#include <avr/interrupt.h>

// define USART registers
// Wifi
#define UCSRA_WIFI UCSR0A
#define UCSRB_WIFI UCSR0B
#define UBRRH_WIFI UBRR0H
#define UBRRL_WIFI UBRR0L
#define RXC_WIFI RXC0
#define UDR_WIFI UDR0
#define UDRE_WIFI UDRE0
#define RXEN_WIFI RXEN0 // Receive enable
#define TXEN_WIFI TXEN0 // Transmit enable
#define RXCIE_WIFI RXCIE0 // Receive interrupt enable

// Transceiver

// Interrupt on UART0 receive
ISR(USART0_RX_vect);

// Initialize UART
void UART_init(unsigned int);
void UART_WiFi_TX (uint8_t);



#endif
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
#define WIFI_RX_vect USART0_RX_vect

// Transceiver
#define UCSRA_TRANS UCSR1A
#define UCSRB_TRANS UCSR1B
#define UBRRH_TRANS UBRR1H
#define UBRRL_TRANS UBRR1L
#define RXC_TRANS RXC1
#define UDR_TRANS UDR1
#define UDRE_TRANS UDRE1
#define RXEN_TRANS RXEN1 // Receive enable
#define TXEN_TRANS TXEN1 // Transmit enable
#define RXCIE_TRANS RXCIE1 // Receive interrupt enable

// Interrupt on UART0 receive
ISR(WIFI_RX_vect);

// Initialize UART
void UART_init(unsigned int);
void UART_WiFi_TX (uint8_t);



#endif
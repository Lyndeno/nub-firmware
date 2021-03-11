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
#define RXEN_WIFI RXEN0 // WiFi receive enable
#define TXEN_WIFI TXEN0 // WiFi transmit enable
#define RXCIE_WIFI RXCIE0 // WiFI receive interrupt enable
#define WIFI_RX_vect USART0_RX_vect // WiFi receive interrupt vector

// Transceiver
#define UCSRA_TRANS UCSR1A
#define UCSRB_TRANS UCSR1B
#define UBRRH_TRANS UBRR1H
#define UBRRL_TRANS UBRR1L
#define RXC_TRANS RXC1
#define UDR_TRANS UDR1
#define UDRE_TRANS UDRE1
#define RXEN_TRANS RXEN1 // Transceiver receive enable
#define TXEN_TRANS TXEN1 // Transceiver transmit enable
#define RXCIE_TRANS RXCIE1 // Transceiver receive interrupt enable
#define TRANS_RX_vect USART1_RX_vect // Transceiver receive interrupt vector

// Initialize UART
void UART_init(unsigned int);
void UART_WiFi_TX (uint8_t);



#endif
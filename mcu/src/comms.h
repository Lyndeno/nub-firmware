#ifndef COMMS_H
#define COMMS_H

#include <avr/interrupt.h>

#define BAUD_WIFI 9600
#define UBRR_WIFI FOSC/16/BAUD_WIFI-1

// define USART registers
// Wifi
#define UCSRA_WIFI UCSR0A
#define UCSRB_WIFI UCSR0B
#define UCSRC_WIFI UCSR0C
#define UBRRH_WIFI UBRR0H
#define UBRRL_WIFI UBRR0L
#define RXC_WIFI RXC0
#define UDR_WIFI UDR0
#define UDRE_WIFI UDRE0
#define U2X_WIFI U2X0
#define RXEN_WIFI RXEN0 // WiFi receive enable
#define TXEN_WIFI TXEN0 // WiFi transmit enable
#define RXCIE_WIFI RXCIE0 // WiFI receive interrupt enable
#define USBS_WIFI USBS0
#define UCSZ0_WIFI UCSZ00
#define WIFI_RX_vect USART0_RX_vect // WiFi receive interrupt vector
// Transceiver
#define UCSRA_TRANS UCSR1A
#define UCSRB_TRANS UCSR1B
#define UCSRC_TRANS UCSR1C
#define UBRRH_TRANS UBRR1H
#define UBRRL_TRANS UBRR1L
#define RXC_TRANS RXC1
#define UDR_TRANS UDR1
#define UDRE_TRANS UDRE1
#define U2X_TRANS U2X1
#define RXEN_TRANS RXEN1 // Transceiver receive enable
#define TXEN_TRANS TXEN1 // Transceiver transmit enable
#define RXCIE_TRANS RXCIE1 // Transceiver receive interrupt enable
#define USBS_TRANS USBS1
#define UCSZ0_TRANS UCSZ10
#define TRANS_RX_vect USART1_RX_vect // Transceiver receive interrupt vector

// Initialize UART

/** 
 * Initializes UART for data transfer between the MCU and ESP8266 WiFi module.
 * 
 * @param ubrr UBRR register value to use for baud rate.
 */
void UART_init(unsigned int);

/** 
 * Writes the given byte to the WiFi transmission register when there is no pending transmission.
 * 
 * @param byte Byte of data to send to WiFi module
 */
void UART_WiFi_TX (uint8_t);

void delay_ms(int t);

uint8_t * setupTransceiver();

void TXWrite(unsigned char c[], uint8_t size, uint16_t uartPort );

#endif
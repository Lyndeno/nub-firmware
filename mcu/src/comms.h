#ifndef COMMS_H
#define COMMS_H

#include <avr/interrupt.h>

#define BAUD_WIFI 9600
#define UBRR_WIFI FOSC/16/BAUD_WIFI-1

// define USART registers
// Wifi
#define UCSRA_WIFI UCSR1A
#define UCSRB_WIFI UCSR1B
#define UBRRH_WIFI UBRR1H
#define UBRRL_WIFI UBRR1L
#define RXC_WIFI RXC1
#define UDR_WIFI UDR1
#define UDRE_WIFI UDRE1
#define RXEN_WIFI RXEN1 // WiFi receive enable
#define TXEN_WIFI TXEN1 // WiFi transmit enable
#define RXCIE_WIFI RXCIE1 // WiFI receive interrupt enable
#define WIFI_RX_vect USART1_RX_vect // WiFi receive interrupt vector

// Transceiver
#define UCSRA_TRANS UCSR0A
#define UCSRB_TRANS UCSR0B
#define UBRRH_TRANS UBRR0H
#define UBRRL_TRANS UBRR0L
#define RXC_TRANS RXC0
#define UDR_TRANS UDR0
#define UDRE_TRANS UDRE0
#define RXEN_TRANS RXEN0 // Transceiver receive enable
#define TXEN_TRANS TXEN0 // Transceiver transmit enable
#define RXCIE_TRANS RXCIE0 // Transceiver receive interrupt enable
#define TRANS_RX_vect USART0_RX_vect // Transceiver receive interrupt vector

// Initialize UART

/** 
 * Initializes UART for data transfer between the MCU and ESP8266 WiFi module.
 * 
 * @param ubrr UBRR register value to use for baud rate.
 */
void UART_WiFi_init(unsigned int);

/** 
 * Writes the given byte to the WiFi transmission register when there is no pending transmission.
 * 
 * @param byte Byte of data to send to WiFi module
 */
void UART_WiFi_TX (uint8_t);
void UART_TRANS_TX (uint8_t byte);

uint8_t * setupTransceiver();


void USART_setup(unsigned int ubrr);

void delay_ms(int t);

void TXWrite(unsigned char c[], uint8_t size, uint16_t uartPort );

uint8_t peekChar(uint8_t);

unsigned char getChar(uint8_t UARTPort);

void skipBuffer(uint8_t uartPort);

//uint8_t parseBufferForVal(int movePointer, uint16_t);
uint8_t Bytes0UnRead();
uint8_t Bytes1UnRead();
//uint16_t parseBufferForPtr(int movePtr, uint16_t);

uint8_t uartReady();



#endif
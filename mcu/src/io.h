#include <avr/io.h>
//#include <util/delay.h>

#ifndef LIBIO_H
#define LIBIO_H

#define DDR(x) (*(&x - 1))     // address of data direction register of port x
#define PORT(x) (*(&x)) 
#define PIN(x) (*(&x - 2))

#define PORT_STATUS_LED PORTB
#define PIN_STATUS_LED PORTB5

// define USART registers
// Wifi
#define UCSRA_WIFI UCSR0A
#define RXC_WIFI RXC0
#define UDR_WIFI UDR0

// Transceiver

/* Code from ECE312 project, adapt as needed
#define PORT_LR PORTD

#define PORT_ENTER PORTB

#define PORT_BUZZER PORTD

#define PIN_LEFT PORTD6

#define PIN_RIGHT PORTD7

#define PIN_ENTER PORTB0

#define PIN_BUZZER PORTD5
*/

/*#ifndef F_CPU
#define F_CPU 1000000
#endif*/

#endif
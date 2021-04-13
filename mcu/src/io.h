#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#ifndef FOSC
#define FOSC 1000000UL
#endif

#include <util/delay.h>

#ifndef LIBIO_H
#define LIBIO_H

#define DDR(x) (*(&x - 1)) // address of data direction register of port x
#define PORT(x) (*(&x)) 
#define PIN(x) (*(&x - 2))

#define PORT_STATUS_LED PORTB
#define PIN_STATUS_LED PORTB5

#define PORT_TEST_LED PORTC
#define PIN_TEST_LED PORTC0

/**/

#endif
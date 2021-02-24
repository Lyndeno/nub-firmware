#include "io.h"

#define F_CPU 16000000UL
#define FOSC 16000000UL
#define BAUD 9600
#define UBRR FOSC/16/BAUD-1

#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "comms.h"


// TODO: Match UART to physical implementation, UART0 and UART1

// IDEA: wifi and transceiver send/receive will probably use very similar code, 
// figure out how to pass UART registers to function

int main (void) {
    UART_init(UBRR);
    DDR(PORT_STATUS_LED) |= (1<<PIN_STATUS_LED);
    DDR(PORT_TEST_LED) |= (1<<PIN_TEST_LED);
    while(1) {
        PORT(PORT_STATUS_LED) |= (1<<PIN_STATUS_LED);
        //PORT(PORT_TEST_LED) &= ~(1<<PIN_TEST_LED);
        //_delay_ms(1000);
        PORT(PORT_STATUS_LED) &= ~(1<<PIN_STATUS_LED);
        //PORT(PORT_TEST_LED) |= (1<<PIN_TEST_LED);

        while (check_buffer() != '\0') {
            while(!( UCSR0A & (1<<UDRE0)));
            UDR0 = read_buffer();
        }
        //_delay_ms(3000);
    }
}

/*
void gpio_init (void) {
    
}*/


/*
// Receive message from WiFi device
// Call appropriate send function when message destination is determined
void wifi_receive (void) {
    // Check if received message waiting in buffer
    if (UCSRA_WIFI & (1<<RXC_WIFI)) {
        // TODO: Handle multi-byte values
        char address[ADDR_LENGTH];
        char message[MESSAGE_LENGTH_MAX];

        // get the recipient information
        for (int addr_byte = 0; addr_byte < ADDR_LENGTH; addr_byte++)
        {
            while (!(UCSRA_WIFI & (1<<RXC_WIFI))); // wait for unread transmission
            address[addr_byte] = UDR0; // read data
            UCSRA_WIFI &= ~(1<<RXC_WIFI); // clear recieve flag 
        }

        // get the message information
        for (int message_byte = 0; message_byte < MESSAGE_LENGTH; message_byte++)
        {
            while (!(UCSRA_WIFI & (1<<RXC_WIFI))); // wait for unread transmission
            message[message_byte] = UDR0; // read data
            UCSRA_WIFI &= ~(1<<RXC_WIFI); // clear recieve flag
        }
        

        // wait for message to come in
        while (!(UCSRA_WIFI & (1<<RXC_WIFI)));
        char message = UDR0;

        route_message(); // will need input args
    }

}*/

/*

// Send message to WiFi device
// Needed parameters: message, device address
void wifi_send (char *message, char *address) {
    // send the recipient information
    for (int addr_byte = 0; addr_byte < ADDR_LENGTH; addr_byte++)
    {
        while (!(UCSRA_WIFI & (1<<RXC_WIFI))); // wait for unread transmission
        
    }

    // send the message information
    for (int message_byte = 0; message_byte < MESSAGE_LENGTH; message_byte++)
    {
        while (!(UCSRA_WIFI & (1<<RXC_WIFI))); // wait for unread transmission
        
    }
}

// Receive message/home/lsanche/Projects/nub-firmware/mcu/src/io.h from transceiver
// Call appropriate send function when message destination is determined
void trans_receive (void) {
    // Check if received message waiting in buffer/home/lsanche/Projects/nub-firmware/mcu/src/io.h
    if (UCSR1A & (1<<RXC1)) {
        // TODO: Handle multi-byte values
        char address = UDR1; // get address info

        // wait for message to come in
        while (!(UCSR1A & (1<<RXC1)));
        char message = UDR1;

        route_message(); // will need input args
    }
}

// Send message through transceiver
// Needed parameters: message, client NUB, device address
void trans_send (void) {

}

// Determine whether message needs to route to connected WiFi device or to NUB
void route_message (void) {
    // decide what to do with message
}

// generate directory of client phones and NUBs they are connected to
// Is NUB mesh topology handled by humpro?
void refresh_topology (void) {

}*/
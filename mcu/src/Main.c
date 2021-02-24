#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

#define F_CPU 16000000UL
#define FOSC 16000000UL
#define BAUD 9600
#define UBRR FOSC/16/BAUD-1

#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// TODO: Might need this
//#include <avr/pgmspace.h>

// TODO: Match UART to physical implementation, UART0 and UART1

// IDEA: wifi and transceiver send/receive will probably use very similar code, 
// figure out how to pass UART registers to function

// lengths to be determined
#define FRAME_LENGTH_MAX 4
#define ADDR_LENGTH 6 // MAC address is 6 bytes
#define TYPE_LENGTH 1 // one char should suffice
#define MESSAGE_LENGTH_MAX (FRAME_LENGTH_MAX - TYPE_LENGTH - ADDR_LENGTH)
#define BUFFER_LENGTH_FRAMES 4 // number of frames
#define BUFFER_LENGTH_BYTES (BUFFER_LENGTH_FRAMES * FRAME_LENGTH_MAX)

static uint16_t buff_read;
static uint16_t buff_write;
static unsigned char buffer0[BUFFER_LENGTH_BYTES];

ISR(USART0_RX_vect) {
    // when interrupt is triggered then write UDR into buffer so we do not lose information
    buffer0[buff_write++] = UDR0;

    PORT(PORT_TEST_LED) ^= (1<<PIN_TEST_LED);

    if (buff_write == (BUFFER_LENGTH_BYTES)) {
        buff_write = 0;
    }
}

unsigned char read_buffer() {
    if (buff_read == (BUFFER_LENGTH_BYTES)) {
        buff_read = 0;
    }
    unsigned char buff_temp = buffer0[buff_read];
    buffer0[buff_read++] = '\0'; // TODO: This might be a bad idea
    return buff_temp;
}

unsigned char check_buffer() {
    if (buff_read == (BUFFER_LENGTH_BYTES)) {
        buff_read = 0;
    }
    return buffer0[buff_read];
}

void UART_init (unsigned int ubrr) {
    // Disable power reduction for USARTS
    //PRR0 |= (1<<PRUSART0)|(1<<PRUSART1);

    // Enable WiFi UART
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)(ubrr);
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0B |= (1<<RXCIE0); // enable receive interrupt for circular buffer
    // default settings are 1 start bit, 8 data bits, no parity, 1 stop bit
    buff_write = 0;
    buff_read = 0;
    memset(buffer0, '\0', sizeof(buffer0));
    sei(); //enable interrupts
}

int main (void) {
    unsigned char data[] = "Hello from ATmega328PB!, this is a test of UART output.\r\n";
    //unsigned char rec_data[] = "Received message: ";
    //unsigned char tempChar;
    UART_init(UBRR);
    DDR(PORT_STATUS_LED) |= (1<<PIN_STATUS_LED);
    DDR(PORT_TEST_LED) |= (1<<PIN_TEST_LED);
    while(1) {
        PORT(PORT_STATUS_LED) |= (1<<PIN_STATUS_LED);
        //PORT(PORT_TEST_LED) &= ~(1<<PIN_TEST_LED);
        //_delay_ms(1000);
        PORT(PORT_STATUS_LED) &= ~(1<<PIN_STATUS_LED);
        //PORT(PORT_TEST_LED) |= (1<<PIN_TEST_LED);
        int i = 0;
        /*while(data[i] != 0) {
            while(!( UCSR0A & (1<<UDRE0)));
            UDR0 = data[i];
            i++;
        }*/

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
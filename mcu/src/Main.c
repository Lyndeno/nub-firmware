#include <avr/io.h>

#define F_CPU 16000000UL

#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: Might need this
//#include <avr/pgmspace.h>

// TODO: Figure this out
//#define F_CPU 1000000



// TODO: Match UART to physical implementation, UART0 and UART1

// IDEA: wifi and transceiver send/receive will probably use very similar code, 
// figure out how to pass UART registers to function

// lengths to be determined
#define ADDR_LENGTH 3
#define MESSAGE_LENGTH 5

int main (void) {
    DDRB |= (1<<DDB5);
    while(1) {
        PORTB |= (1<<PORTB5);
        _delay_ms(1000);
        PORTB &= ~(1<<PORTB5);
        _delay_ms(1000);
    }
}

/*
void gpio_init (void) {
    
}

void UART_init (void) {
    // Disable power reduction for USARTS
    PRR0 |= (1<<PRUSART0)|(1<<PRUSART1);
}

// Receive message from WiFi device
// Call appropriate send function when message destination is determined
void wifi_receive (void) {
    // Check if received message waiting in buffer
    if (UCSR0A & (1<<RXC0)) {
        // TODO: Handle multi-byte values
        char address[ADDR_LENGTH];
        char message[MESSAGE_LENGTH];

        // get the recipient information
        for (int addr_byte = 0; addr_byte < ADDR_LENGTH; addr_byte++)
        {
            while (!(UCSR0A & (1<<RXC0))); // wait for unread transmission
            address[addr_byte] = UDR0; // read data
            UCSR0A &= ~(1<<RXC0); // clear recieve flag 
        }

        // get the message information
        for (int message_byte = 0; message_byte < MESSAGE_LENGTH; message_byte++)
        {
            while (!(UCSR0A & (1<<RXC0))); // wait for unread transmission
            message[message_byte] = UDR0; // read data
            UCSR0A &= ~(1<<RXC0); // clear recieve flag
        }
        

        // wait for message to come in
        while (!(UCSR0A & (1<<RXC0)));
        char message = UDR0;

        route_message(); // will need input args
    }

}

// Send message to WiFi device
// Needed parameters: message, device address
void wifi_send (char *message, char *address) {
    // send the recipient information
    for (int addr_byte = 0; addr_byte < ADDR_LENGTH; addr_byte++)
    {
        while (!(UCSR0A & (1<<RXC0))); // wait for unread transmission
        
    }

    // send the message information
    for (int message_byte = 0; message_byte < MESSAGE_LENGTH; message_byte++)
    {
        while (!(UCSR0A & (1<<RXC0))); // wait for unread transmission
        
    }
}

// Receive message from transceiver
// Call appropriate send function when message destination is determined
void trans_receive (void) {
    // Check if received message waiting in buffer
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
#include <avr/io.h>

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

}

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
    if (UCSR0A & (1<<7 /* TODO: Switch this to name of bit */ )) {
        // TODO: Handle multi-byte values
        char address = UDR0; // get address info

        // wait for message to come in
        while (!(UCSR0A & (1<<7 /* TODO: Switch this to name of bit */ )));
        char message = UDR0;

        route_message(); // will need input args
    }

}

// Send message to WiFi device
// Needed parameters: message, device address
void wifi_send (void) {
    
}

// Receive message from transceiver
// Call appropriate send function when message destination is determined
void trans_receive (void) {
    // Check if received message waiting in buffer
    if (UCSR0A & (1<<7 /* TODO: Switch this to name of bit */ )) {
        // TODO: Handle multi-byte values
        char address = UDR0; // get address info

        // wait for message to come in
        while (!(UCSR0A & (1<<7 /* TODO: Switch this to name of bit */ )));
        char message = UDR0;

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
    if (/* Recipient is on another NUB */) {

    } else if (/* Recipient is on this NUB */) {

    }
}

// generate directory of client phones and NUBs they are connected to
// Is NUB mesh topology handled by humpro?
void refresh_topology (void) {

}
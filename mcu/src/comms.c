#include <string.h>
#include "buffer.h"
#include "comms.h"
#include "io.h"

ISR(USART0_RX_vect) {
    // when interrupt is triggered then write UDR into buffer so we do not lose information
    buffer0.buff[buffer0.tail++] = UDR0;
    buffer0.free--;

    PORT(PORT_TEST_LED) ^= (1<<PIN_TEST_LED);

    if (buffer0.tail >= buffer0.max) {
        buffer0.tail = 0;
    }
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
    //memset(buffer0, '\0', sizeof(buffer0));
    sei(); //enable interrupts
}


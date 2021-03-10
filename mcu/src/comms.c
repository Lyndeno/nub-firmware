#include <string.h>
#include "comms.h"
#include "io.h"

size_t buff_read;
size_t buff_write;
volatile unsigned char buffer0[BUFFER_LENGTH_BYTES];

ISR(USART0_RX_vect) {
    // when interrupt is triggered then write UDR into buffer so we do not lose information
    buffer0[buff_write++] = UDR0;

    PORT(PORT_TEST_LED) ^= (1<<PIN_TEST_LED);

    if (buff_write == (BUFFER_LENGTH_BYTES)) {
        buff_write = 0;
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
    buff_write = 0;
    buff_read = 0;
    //memset(buffer0, '\0', sizeof(buffer0));
    sei(); //enable interrupts
}

unsigned char read_buffer() {
    //while(check_buffer() == '\0');
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
#include <string.h>
#include "buffer.h"
#include "comms.h"
#include "io.h"

ISR(USART0_RX_vect) {
    // when interrupt is triggered then write UDR into buffer so we do not lose information
    buff_wifi_rx.buff[buff_wifi_rx.tail++] = UDR0;
    buff_wifi_rx.free--;

    PORT(PORT_TEST_LED) ^= (1<<PIN_TEST_LED);

    if (buff_wifi_rx.tail >= buff_wifi_rx.max) {
        buff_wifi_rx.tail = 0;
    }
}

void UART_init (unsigned int ubrr) {
    // Disable power reduction for USARTS
    //PRR0 |= (1<<PRUSART0)|(1<<PRUSART1);

    // Enable WiFi UART
    UBRRH_WIFI = (unsigned char)(ubrr>>8);
    UBRRL_WIFI = (unsigned char)(ubrr);
    UCSRB_WIFI = (1<<RXEN_WIFI)|(1<<TXEN_WIFI);
    UCSRB_WIFI |= (1<<RXCIE_WIFI); // enable receive interrupt for circular buffer
    // default settings are 1 start bit, 8 data bits, no parity, 1 stop bit
    //memset(buffer0, '\0', sizeof(buffer0));
    sei(); //enable interrupts
}

// Wait for TX register to clear then transmit a byte
void UART_WiFi_TX (uint8_t byte) {
    while(!( UCSRA_WIFI & (1<<UDRE_WIFI)));
    UDR_WIFI = byte;
}


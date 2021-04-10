#include <string.h>
#include "buffer.h"
#include "comms.h"
#include "io.h"


ISR(WIFI_RX_vect) {
    // when interrupt is triggered then write UDR into buffer so we do not lose information
    buff_wifi_rx.buff[buff_wifi_rx.tail++] = UDR0;
    buff_wifi_rx.free--;

    PORT(PORT_TEST_LED) ^= (1<<PIN_TEST_LED);

    if (buff_wifi_rx.tail >= buff_wifi_rx.size) {
        buff_wifi_rx.tail = 0;
    }
}

ISR(TRANS_RX_vect) {
    // when interrupt is triggered then write UDR into buffer so we do not lose information
    buff_trans_rx.buff[buff_trans_rx.tail++] = UDR1;
    buff_trans_rx.free--;

    PORT(PORT_TEST_LED) ^= (1<<PIN_TEST_LED);

    if (buff_trans_rx.tail >= buff_trans_rx.size) {
        buff_trans_rx.tail = 0;
    }
}

/** 
 * Initializes a given buffer for use. This function sets the baud rate, enables transmission and reception,
 * and enables a receive interrupt for UART.
 */
void UART_WiFi_init (unsigned int ubrr) {
    // Disable power reduction for USARTS
    //PRR0 |= (1<<PRUSART0)|(1<<PRUSART1);

    // Enable WiFi UART
    UBRRH_WIFI = (unsigned char)(ubrr>>8); // Set high bits of UBRR for baud rate
    UBRRL_WIFI = (unsigned char)(ubrr); // Set low bits of UBRR for baud
    UCSRB_WIFI = (1<<RXEN_WIFI)|(1<<TXEN_WIFI); // Enable receiving and transmission on UART
    UCSRB_WIFI |= (1<<RXCIE_WIFI); // enable receive interrupt for circular buffer

    // Enable WiFi UART
    UBRRH_TRANS = (unsigned char)(ubrr>>8); // Set high bits of UBRR for baud rate
    UBRRL_TRANS = (unsigned char)(ubrr); // Set low bits of UBRR for baud
    UCSRB_TRANS = (1<<RXEN_TRANS)|(1<<TXEN_TRANS); // Enable receiving and transmission on UART
    UCSRB_TRANS |= (1<<RXCIE_TRANS); // enable receive interrupt for circular buffer

    sei(); //enable interrupts
}

/** 
 * Transmit byte to WiFi modules. This function waits for the ATmega to finish transmitting the last byte before
 * attempting another transfer.
 */
void UART_WiFi_TX (uint8_t byte) {
    while(!( UCSRA_WIFI & (1<<UDRE_WIFI))); // Wait for TX to finish
    UDR_WIFI = byte; // Write for TX
}

void UART_TRANS_TX (uint8_t byte) {
    while(!( UCSRA_TRANS & (1<<UDRE_TRANS))); // Wait for TX to finish
    UDR_TRANS = byte; // Write for TX
}


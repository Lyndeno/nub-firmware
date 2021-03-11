#include "io.h"

#define F_CPU 16000000UL
#define FOSC 16000000UL
#define BAUD 9600
#define UBRR FOSC/16/BAUD-1

#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "comms.h"
#include "buffer.h"


// TODO: Match UART to physical implementation, UART0 and UART1

// IDEA: wifi and transceiver send/receive will probably use very similar code, 
// figure out how to pass UART registers to function

void handle_message(circular_buf * );
void check_frame (circular_buf * );

int main (void) {
    // Initialize buffers first as UART needs them
    init_buffer(&buff_wifi_rx);
    UART_init(UBRR);
    DDR(PORT_STATUS_LED) |= (1<<PIN_STATUS_LED);
    DDR(PORT_TEST_LED) |= (1<<PIN_TEST_LED);

    while (1) {
        PORT(PORT_STATUS_LED) ^= (1<<PIN_STATUS_LED);

        check_frame(&buff_wifi_rx);

        //_delay_ms(3000);
        
    }
}

void check_frame (circular_buf *buffer_ptr) {
    if(buffer_ptr->free < buffer_ptr->max) {
        uint8_t data_len;
        if (read_buffer(buffer_ptr) == 0x02) {
            data_len = read_buffer(buffer_ptr);
            switch(read_buffer(buffer_ptr)) {
                case 0x01: // Text message
                    handle_message(buffer_ptr);
                    break;
                case 0x02: // New device (dis)connected
                    break;
                case 0x03: // NUB topology adjustment
                    break;
                case 0x04: // Acknowledgements
                    break;
            }
        }
    }
}


void handle_message(circular_buf *buffer_ptr) {
    uint16_t mess_len;
    mess_len = (read_buffer(buffer_ptr) << 8);
    mess_len |= read_buffer(buffer_ptr);

    for (size_t i = 0; i < mess_len; i++) {
        UART_WiFi_TX(read_buffer(buffer_ptr));
    }
}
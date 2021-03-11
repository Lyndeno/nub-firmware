#include "routing.h"
#include <stdint.h>
#include "comms.h"

void read_frame (circular_buf *buffer_ptr) {
    if(buffer_ptr->free < buffer_ptr->size) {
        uint8_t data_len;
        if (read_buffer(buffer_ptr) == 0x02) {
            data_len = read_buffer(buffer_ptr); // TODO: Can not remember what this is for.
            switch(read_buffer(buffer_ptr)) {
                case 0x01: // Text message
                    route_message(buffer_ptr);
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


void route_message(circular_buf *buffer_ptr) {
    uint16_t mess_len; // variable to store message length
    mess_len = (read_buffer(buffer_ptr) << 8); // Read in high bits
    mess_len |= read_buffer(buffer_ptr); // Read in low bits

    for (size_t i = 0; i < mess_len; i++) { // loop through length of data
        UART_WiFi_TX(read_buffer(buffer_ptr)); // Write to TX
    }
}
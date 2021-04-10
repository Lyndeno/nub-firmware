#include "routing.h"
#include <stdint.h>
#include "comms.h"

static wifi_device connected_devices[ESP_MAX_CONN];

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
                    handle_connection(buffer_ptr);
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

    uint8_t sourceMAC[6];
    read_MAC(buffer_ptr, sourceMAC);
    int connected = find_MAC(sourceMAC);

    if (connected < 0) {
        UART_TRANS_TX(0x02);
        UART_TRANS_TX(0x00);
        UART_TRANS_TX(0x01);
        UART_TRANS_TX( (uint8_t)(mess_len >> 8));
        UART_TRANS_TX( (uint8_t)(mess_len & 0x00FF));
        for (size_t i = 0; i < MAC_LENGTH; i++) {
            UART_TRANS_TX(sourceMAC[i]);
        }
        for (size_t i = 0; i < mess_len - MAC_LENGTH; i++) { // loop through length of data
            UART_TRANS_TX(read_buffer(buffer_ptr)); // Write to TX
        }

    } else {
        UART_WiFi_TX(0x02);
        UART_WiFi_TX(0x00);
        UART_WiFi_TX(0x01);
        UART_WiFi_TX( (uint8_t)(mess_len >> 8));
        UART_WiFi_TX( (uint8_t)(mess_len & 0x00FF));
        for (size_t i = 0; i < MAC_LENGTH; i++) {
            UART_WiFi_TX(sourceMAC[i]);
        }
        for (size_t i = 0; i < mess_len - MAC_LENGTH; i++) { // loop through length of data
            UART_WiFi_TX(read_buffer(buffer_ptr)); // Write to TX
        }
    }

    
}

void handle_connection (circular_buf *buffer_ptr) {
    wifi_device mod_device;
    mod_device.mac = malloc(MAC_LENGTH * sizeof(uint8_t));
    switch (read_buffer(buffer_ptr)) {
    case 0x01: // Connected
        read_MAC(buffer_ptr, mod_device.mac);

        for (size_t dev_index = 0; dev_index < ESP_MAX_CONN; dev_index++) {
            if (connected_devices[dev_index].state == Disconnected) {
                copy_MAC(mod_device.mac,connected_devices[dev_index].mac);
                connected_devices[dev_index].state = Connected;
                break;
            }
        }
        break;
    
    case 0x02: // Disconnected
        for (size_t dev_index = 0; dev_index < ESP_MAX_CONN; dev_index++) {
            if (compare_MAC( connected_devices[dev_index].mac, mod_device.mac )) {
                connected_devices[dev_index].state = Disconnected;
            }
        }
        break;

    default:
        break;
    }

    free(mod_device.mac);
}

void init_device_table(void) {
    for (size_t i = 0; i < ESP_MAX_CONN; i++) {
        connected_devices[i].mac = malloc(MAC_LENGTH * sizeof(uint8_t));
        connected_devices[i].state = Disconnected;
    }
}

bool compare_MAC (uint8_t *mac1, uint8_t *mac2) {
    bool match = true;
    for (size_t mac_index = 0; mac_index < MAC_LENGTH; mac_index++) {
        match = mac1[mac_index] == mac2[mac_index];
        if (!match) {break;}
    }
    return match;
}

void copy_MAC (uint8_t *mac_in, uint8_t *mac_out) {
    for (size_t mac_index = 0; mac_index < MAC_LENGTH; mac_index++) {
        mac_out[mac_index] = mac_in[mac_index];
    }
}

void read_MAC (circular_buf *buffer_ptr, uint8_t *mac_out) {
    for (size_t mac_index = 0; mac_index < MAC_LENGTH; mac_index++) {
        mac_out[mac_index] = read_buffer(buffer_ptr);
    }
}

int find_MAC (uint8_t *mac) {
    int result = -1;
    for (size_t i = 0; i < ESP_MAX_CONN; i++) {
        if ( compare_MAC(mac, connected_devices[i].mac) ) {
            return i;
        }
    }
    return result;
}
#include "routing.h"
#include "freertos/task.h"

QueueHandle_t q_uart_rx_bytes = NULL; // bytes received from UART
QueueHandle_t q_uart_tx_bytes = NULL; // bytes to send through UART
QueueHandle_t q_wifi_rx_frames = NULL; // frame of bytes received from WiFi
QueueHandle_t q_wifi_tx_frames = NULL; // frame of bytes to send through WiFI
QueueHandle_t q_wifi_state = NULL;

static wifi_device wifi_connection_table[ESP_WIFI_MAX_CONN]; // hold the information for currently connected devices

const TickType_t block_time = pdMS_TO_TICKS( 1000 );

void handle_bytes_task (void *pvParamters) {
    uint8_t temp_rx;
    uint16_t mess_len;
    

    while (1) {
        if( xQueueReceive(q_uart_rx_bytes, &temp_rx, block_time)) { // TODO: Maybe change this to a while loop for cleanliness
            if ( temp_rx == 0x02 ) { // Check for beginning of NUB data frame
                while ( (xQueueReceive(q_uart_rx_bytes, &temp_rx, block_time)) != 1); // wait for data_len byte
                while ( (xQueueReceive(q_uart_rx_bytes, &temp_rx, block_time)) != 1); // wait for message_type byte

                // Add logic for different message types when needed
                while ( (xQueueReceive(q_uart_rx_bytes, &temp_rx, block_time)) != 1); // wait for first byte of message length
                mess_len = (temp_rx << 8);
                while ( (xQueueReceive(q_uart_rx_bytes, &temp_rx, block_time)) != 1); // wait for second byte of message length
                mess_len |= temp_rx;

                message_frame tx_frame;
                tx_frame.data = malloc(mess_len * sizeof(uint8_t));
                tx_frame.len = mess_len;
                for (size_t i = 0; i < tx_frame.len; i++) {
                    // TODO: Might be able to pass the array pointer to xqueuereceive
                    while ( (xQueueReceive(q_uart_rx_bytes, &temp_rx, block_time)) != 1);
                    tx_frame.data[i] = temp_rx;
                }

                while(xQueueSendToBack(q_wifi_tx_frames, &tx_frame, block_time) != 1); // send to wifi queue until successful
            }
        }
    }

    vTaskDelete(NULL);
}

void handle_frames_task (void *pvParameters) {
    message_frame rx_frame;

    while (1) {
        if( xQueueReceive(q_wifi_rx_frames, &rx_frame, block_time) != 1) {
            /*
             * TODO: Move this documentation elsewhere
             * Frame Data for transmission between WiFi device and ESP:
             * 
             * First Byte: message type
             *  - 0x01 for standard message
             *  - 0x02 for device (dis)connect from NUB (only received if device manually disconnects, handle WiFi disconnection from WiFi event handler)
             * 
             * For device (dis)connection:
             *  - Second byte, 0x01 for connect, 0x02 for disconnect
             *  - Next six bytes: MAC address of device in question
             */ 
            switch (rx_frame.data[1])
            {
            case 0x01: // Standard message
                handle_message_frame(&rx_frame);
                break;

            case 0x02: // device connect/disconnected

                break;
            }
        }
    }
}

void handle_message_frame (message_frame *rx_frame) {
    tx_byte(0x02); // NUB header
    tx_byte(0x00); // data len
    tx_byte(0x01); // msg type
    tx_byte(((uint16_t)(rx_frame->len)) >> 8); // first byte of length
    tx_byte(((uint16_t)(rx_frame->len)) & 0x00FF); // second byte of length

    // send message to uart queue
    for (size_t i = 0; i < rx_frame->len; i++) {
        tx_byte(rx_frame->data[i]);
    }
}

// Add/remove device from table and report to MCU
void device_table_task (void *pvParameters) {
    q_wifi_state = xQueueCreate(32, sizeof(wifi_device));
    wifi_device new_state;

    // initialize the device table
    for (size_t i = 0; i < ESP_WIFI_MAX_CONN; i++) {
        wifi_connection_table[i].state = Disconnected;
        wifi_connection_table[i].mac = pvPortMalloc(MAC_LENGTH * sizeof(uint8_t));
    }

    while (1) {
        if (xQueueReceive(q_wifi_state, &new_state, portMAX_DELAY)) {
            switch (new_state.state)
            {
            case Connected:
                /*
                 * TODO: Add handling for duplicate connection notices, i.e. same device sends connection frame twice.
                 * To prevent the table from filling up.
                 */ 
                for (size_t dev_index = 0; dev_index < ESP_WIFI_MAX_CONN; dev_index++) {
                    if (wifi_connection_table[dev_index].state == Disconnected) {
                        copy_MAC(new_state.mac,wifi_connection_table[dev_index].mac);
                        wifi_connection_table[dev_index].netaddr = new_state.netaddr;
                        wifi_connection_table[dev_index].state = Connected;
                    }
                }
                break;

            case Disconnected:
                for (size_t dev_index = 0; dev_index < ESP_WIFI_MAX_CONN; dev_index++) {
                    if (compare_MAC( wifi_connection_table[dev_index].mac, new_state.mac )) {
                        wifi_connection_table[dev_index].state = Disconnected;
                    }
                }
                break;
            }
            vPortFree(new_state.mac);
        }
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

void tx_byte(uint8_t byte) {
    while( xQueueSendToBack(q_uart_tx_bytes, &byte, block_time) != 1);
}

void rx_byte(uint8_t *byte_addr) {
    while( xQueueReceive(q_uart_rx_bytes, byte_addr, block_time) != 1);
}
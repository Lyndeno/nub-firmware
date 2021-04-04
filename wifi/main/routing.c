#include "routing.h"
#include "freertos/task.h"

//#define rx_byte(x) xQueueReceive(q_uart_rx_bytes, x, 5000 / portTICK_RATE_MS)
#define rx_byte(x) xQueueReceive(q_uart_rx_bytes, x, portMAX_DELAY)


QueueHandle_t q_uart_rx_bytes = NULL; // bytes received from UART
QueueHandle_t q_uart_tx_bytes = NULL; // bytes to send through UART
QueueHandle_t q_wifi_rx_frames = NULL; // frame of bytes received from WiFi
QueueHandle_t q_wifi_tx_frames = NULL; // frame of bytes to send through WiFI
QueueHandle_t q_wifi_state = NULL;

static wifi_device wifi_connection_table[ESP_WIFI_MAX_CONN]; // hold the information for currently connected devices

const TickType_t block_time = portMAX_DELAY;//pdMS_TO_TICKS( 1000 );

void route_init(void) {
    q_wifi_rx_frames = xQueueCreate(256, sizeof(message_frame));
    q_wifi_tx_frames = xQueueCreate(256, sizeof(message_frame));
    q_uart_rx_bytes = xQueueCreate(256, sizeof(uint8_t));
    q_uart_tx_bytes = xQueueCreate(256, sizeof(uart_frame));
    xTaskCreate(handle_bytes_task, "handle_bytes_task", 256, NULL, 5, NULL);
    xTaskCreate(handle_frames_task, "handle_frames_task", 256, NULL, 5, NULL);
    xTaskCreate(device_table_task, "device_table_task", 256, NULL, 5, NULL);
}

void handle_bytes_task (void *pvParamters) {
    uint8_t temp_rx;
    uint16_t mess_len;
    message_frame tx_frame;
    bool success;
    

    while (1) {
        success = true;
        // TODO: Clean up this mess
        if ( rx_byte(&temp_rx) != pdTRUE ) continue; // check for first byte
        if ( temp_rx == 0x02 ) {
            if ( !rx_byte(&temp_rx) ) continue; // get data_len
            if ( !rx_byte(&temp_rx) ) continue; // get message type
            switch (temp_rx) { // Check message type
                case 0x01: { // Text message
                    if ( !rx_byte(&temp_rx) ) continue; // get first byte of message length
                    mess_len = (temp_rx << 8);
                    if ( !rx_byte(&temp_rx) ) continue; // then get the second byte
                    mess_len |= temp_rx;

                    //tx_frame.len = mess_len;
                    tx_frame.len = 17;
                    tx_frame.data = pvPortMalloc(tx_frame.len * sizeof(uint8_t));

                    for (size_t i = 0; i < tx_frame.len; i++) {
                        if ( !rx_byte(&(tx_frame.data[i])) ) {
                            success = false;
                            break;
                        }
                    }

                    if (!success) {
                        vPortFree(tx_frame.data);
                        continue;
                    } else {
                        /*
                        * Message format:
                        * After all of the header data, the data that resides in the message bytes is:
                        * 1. Destination Address
                        * 2. Source Address
                        * 3. Message 
                        */
                        tx_frame.devaddr = get_sock(&(tx_frame.data[5]));

                        xQueueSendToBack(q_wifi_tx_frames, &tx_frame, portMAX_DELAY);
                    }

                    

                    break;
                }
                
                default: {
                    break;
                }
            }
        }
    }

    vTaskDelete(NULL);
}

void handle_frames_task (void *pvParameters) {
    message_frame rx_frame;

    while (1) {
        if( xQueueReceive(q_wifi_rx_frames, &rx_frame, block_time)) {
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
            switch (rx_frame.data[0]) {
                case 0x01: { // Standard message
                    handle_message_frame(&rx_frame);
                    break;
                }

                case 0x02: {// device connect/disconnected 
                    handle_connection_frame(&rx_frame);
                    break;
                }
            }
        }
    }
}

void handle_message_frame (message_frame *rx_frame) {
    uart_frame tx_bytes;
    tx_bytes.len = rx_frame->len + 5; // 5 bytes before message starts
    tx_bytes.data = pvPortMalloc(tx_bytes.len * sizeof(uint8_t));
    tx_bytes.data[0] = 0x02; // NUB header
    tx_bytes.data[1] = 0x00; // data len
    tx_bytes.data[2] = 0x01; // msg type
    tx_bytes.data[3] = (rx_frame->len) >> 8; // first byte of length
    tx_bytes.data[4] = (rx_frame->len) & 0x00FF; // second byte of length

    // send message to uart queue
    for (size_t i = 0; i < rx_frame->len; i++) {
        tx_bytes.data[i + 5] = rx_frame->data[i];
    }
    xQueueSendToBack(q_uart_tx_bytes, &tx_bytes, portMAX_DELAY);
}

void handle_connection_frame (message_frame *rx_frame) {
    wifi_device mod_device;
    mod_device.mac = (uint8_t *)pvPortMalloc(MAC_LENGTH * sizeof(uint8_t));
    copy_MAC(&(rx_frame->data[2]), mod_device.mac);
    switch (rx_frame->data[1]) {
        case 0x01: { // Connect
            mod_device.netaddr = rx_frame->devaddr;
            mod_device.state = Connected;
            xQueueSendToBack(q_wifi_state, &mod_device, portMAX_DELAY);
            break;
        }
        case 0x02: { // Disconnect
            mod_device.state = Disconnected;
            xQueueSendToBack(q_wifi_state, &mod_device, portMAX_DELAY);
            break;
        }
        default: { // Invalid
            vPortFree(mod_device.mac);
            break;
        }
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
                        //break; // TODO: Why does this cause a crash at boot???
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

struct sockaddr_in get_sock(uint8_t *mac) {
    size_t i;
    for (i = 0; i < ESP_WIFI_MAX_CONN; i++) {
        if ( compare_MAC(mac, wifi_connection_table[i].mac) && wifi_connection_table[i].state == Connected ) {
            return wifi_connection_table[i].netaddr;
        }
    }
    return wifi_connection_table[i].netaddr;

    //TODO: Handle no existing devices instead of returning that last one
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

/*long rx_byte(uint8_t *byte_addr) {
    return xQueueReceive(q_uart_rx_bytes, byte_addr, portMAX_DELAY);

}*/
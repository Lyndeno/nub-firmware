#include "routing.h"
#include "freertos/task.h"

QueueHandle_t q_uart_rx_bytes = NULL; // bytes received from UART
QueueHandle_t q_uart_tx_bytes = NULL; // bytes to send through UART
QueueHandle_t q_wifi_rx_frames = NULL; // frame of bytes received from WiFi
QueueHandle_t q_wifi_tx_frames = NULL; // frame of bytes to send through WiFI

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
            
            default:
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
void handle_device_table (message_frame *frame) {

}

void tx_byte(uint8_t byte) {
    while( xQueueSendToBack(q_uart_tx_bytes, &byte, block_time) != 1);
}

void rx_byte(uint8_t *byte_addr) {
    while( xQueueReceive(q_uart_rx_bytes, byte_addr, block_time) != 1);
}
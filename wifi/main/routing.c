#include "routing.h"
#include "freertos/task.h"

void bytes_to_message_task (void *pvParamters) {
    uint8_t temp_rx;
    uint16_t mess_len;
    const TickType_t block_time = pdMS_TO_TICKS( 1000 );

    for(;;) {
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
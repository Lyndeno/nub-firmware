#include "comms.h"
#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "routing.h"
#include "freertos/task.h"

//static QueueHandle_t s_uart0Queue;

void UART_init (void) {
    uart_config_t uart_config = {
        .baud_rate = 9600, // match this with MCU baud
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_NUM_0, &uart_config);

    //uart_driver_install(UART_NUM_0, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);

    // TODO: Research why setting the tasks priority of these differently from other tasks messes things up
    xTaskCreate(UART_rx_task, "UART_rx_task", 512, NULL, 5, NULL); //change as needed
    xTaskCreate(UART_tx_task, "UART_tx_task", 512, NULL, 5, NULL);
}

void UART_rx_task (void *pvParameters) {
    uint8_t *rx_buff = (uint8_t *) pvPortMalloc(BUF_SIZE * sizeof(uint8_t));

    while (1) {
        // Read Data
        size_t len = uart_read_bytes(UART_NUM_0, rx_buff, BUF_SIZE, 20 / portTICK_RATE_MS);
        for (size_t i = 0; i < len; i++) {
            while (xQueueSendToBack(q_uart_rx_bytes, &rx_buff[i], portMAX_DELAY) != 1);
        }
    }
}

void UART_tx_task (void *pvParameters) {
    uart_frame tempframe;

    while (1) {
        if (xQueueReceive(q_uart_tx_bytes, &tempframe, portMAX_DELAY)) {
            uart_write_bytes(UART_NUM_0, (const char *) (tempframe.data), tempframe.len);
            vPortFree(tempframe.data);
        }
    }
}
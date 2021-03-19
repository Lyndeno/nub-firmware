#include "comms.h"
#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "routing.h"

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

    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);

    q_uart_rx_bytes = xQueueCreate(256, sizeof(uint8_t));
    q_uart_tx_bytes = xQueueCreate(256, sizeof(uint8_t));

    //xTaskCreate(uartEventTask, "uartEventTask", 2048, NULL, 12, NULL); //change as needed
}
#include "comms.h"
#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "freertos/queue.h"

static QueueHandle_t s_uart0Queue;

void UART_init (void) {
    uart_config_t uart_config = {
        .baud_rate = 9600, // match this with MCU baud
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_NUM_0, &uart_config);

    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, BUF_SIZE * 2, 100, &s_uart0Queue, 0);

    //xTaskCreate(uartEventTask, "uartEventTask", 2048, NULL, 12, NULL); //change as needed
}
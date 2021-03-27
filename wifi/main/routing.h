#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "lwip/inet.h"


extern QueueHandle_t q_uart_rx_bytes; // bytes received from UART
extern QueueHandle_t q_uart_tx_bytes; // bytes to send through UART
extern QueueHandle_t q_wifi_rx_frames; // frame of bytes received from WiFi
extern QueueHandle_t q_wifi_tx_frames; // frame of bytes to send through WiFI

// Simple message frame
typedef struct message_frame {
    uint8_t *data;
    size_t len;
    in_addr_t ip_addr;
} message_frame;

void tx_byte(uint8_t);
void rx_byte(uint8_t * );
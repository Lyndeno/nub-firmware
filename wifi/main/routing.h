#ifndef ROUTING_H
#define ROUTING_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "network.h"
#include <stdbool.h>
#include "lwip/inet.h"

#define MAC_LENGTH 6


extern QueueHandle_t q_uart_rx_bytes; // bytes received from UART
extern QueueHandle_t q_uart_tx_bytes; // bytes to send through UART
extern QueueHandle_t q_wifi_rx_frames; // frame of bytes received from WiFi
extern QueueHandle_t q_wifi_tx_frames; // frame of bytes to send through WiFI
extern QueueHandle_t q_wifi_state; // queue to update device table when a device connects or disconnects

typedef enum connection_state {
    Connected, 
    Disconnected
} connection_state;

// Simple message frame
typedef struct message_frame {
    uint8_t *data;
    size_t len;
    //in_addr_t ip_addr;
    //in_port_t port;
    struct sockaddr_in devaddr;
} message_frame;

typedef struct uart_frame {
    uint8_t *data;
    size_t len;
} uart_frame;

typedef struct wifi_device {
    uint8_t *mac;
    struct sockaddr_in netaddr;
    connection_state state;
} wifi_device;

/*
typedef struct wifi_state {
    uint8_t mac[MAC_LENGTH];
    connection_state state;
} wifi_state;
*/
/*
typedef struct wifi_table {
    wifi_device device[ESP_WIFI_MAX_CONN];
    bool occupied[ESP_WIFI_MAX_CONN]; 
} wifi_table;
*/

void route_init(void);
long rx_byte(uint8_t *byte_addr);
void handle_message_frame (message_frame *rx_frame);
bool compare_MAC (uint8_t *mac1, uint8_t *mac2);
void copy_MAC (uint8_t *mac_in, uint8_t *mac_out);
void handle_connection_frame (message_frame *rx_frame);

// tasks
void handle_bytes_task (void *pvParamters);
void handle_frames_task (void *pvParameters);
void device_table_task (void *pvParameters);

#endif
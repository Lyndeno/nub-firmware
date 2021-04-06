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

/**
 * Initialize the routing queues and tasks
 */
void route_init(void);

/**
 * Take message frame and call appropriate function for the message type
 * 
 * @param rx_frame pointer to message frame structure containing message and length
 */
void handle_message_frame (message_frame *rx_frame);

/**
 * Take two mac addresses and compare them
 * 
 * @param mac1 Pointer to first MAC address to compare
 * @param mac2 Pointer to second MAC address to compare
 * 
 * @return true when MAC addresses match, false otherwise.
 */
bool compare_MAC (uint8_t *mac1, uint8_t *mac2);

/**
 * Copy MAC address from one location to another
 * 
 * @param mac_in Pointer to MAC address to be copied
 * @param mac_out Pointer to location to copy MAC address
 */
void copy_MAC (uint8_t *mac_in, uint8_t *mac_out);

/**
 * Parse connection frame and add/remove device to/from connection table
 * 
 * @param rx_frame Pointer to frame containing state information.
 */
void handle_connection_frame (message_frame *rx_frame);

/**
 * Get socket address information from MAC address
 * 
 * @param mac Pointer to MAC address we want the socket address for
 * 
 * @return Returns sockaddr_in socket address information found from connection table.
 */
struct sockaddr_in get_sock(uint8_t *mac);

// tasks

/**
 * Task: Retrieve bytes from UART receive queue and construct a message frame.
 */
void handle_bytes_task (void *pvParamters);

/**
 * Task: Retrieve message frames from WiFi reception queue and parse them into byte arrays for UART
 */
void handle_frames_task (void *pvParameters);

/**
 * Task: Retrieve device state change frames from WiFi queue and add or
 * remove device from connection table for routing.
 */
void device_table_task (void *pvParameters);

#endif
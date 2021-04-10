#ifndef ROUTING_H
#define ROUTING_H

#include "buffer.h"

#define ESP_MAX_CONN 4
#define MAC_LENGTH 6

typedef enum connection_state {
    Connected, 
    Disconnected
} connection_state;

typedef struct wifi_device {
    uint8_t *mac;
    connection_state state;
} wifi_device;

void read_frame (circular_buf * );

void route_message(circular_buf * );

void handle_connection (circular_buf *buffer_ptr);

void init_device_table(void);

bool compare_MAC (uint8_t *mac1, uint8_t *mac2);

void copy_MAC (uint8_t *mac_in, uint8_t *mac_out);

void read_MAC (circular_buf *buffer_ptr, uint8_t *mac_out);

int find_MAC (uint8_t *mac);

#endif
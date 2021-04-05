#ifndef NETWORK_H
#define NETWORK_H

#define ESP_WIFI_MAX_CONN 4

extern const char *TAG;

/**
 * Initialize WiFi access point.
 * 
 * Network information such as SSID, encryption,
 * and password are defined in source file.
 */
void wifi_init_softap(void);

/**
 * Setup the udp server task
 */
void udp_server_init(void);

/**
 * Task: Handle incoming UDP packets from connected WiFi stations
 */
void udp_recv_task(void *pvParameters);

/**
 * Task: Handle message frames from TX queue. Use the message and socket information
 * to send the message to the proper recipient.
 */
void udp_trans_task(void *pvParameters);

/**
 * Set up UDP server and set the socket variable for network tasks.
 */
void udp_server_task(void *);


#endif
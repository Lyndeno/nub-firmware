#ifndef NETWORK_H
#define NETWORK_H

#define ESP_WIFI_MAX_CONN 4

extern const char *TAG;

void wifi_init_softap(void);
void udp_server_task(void *);

#endif
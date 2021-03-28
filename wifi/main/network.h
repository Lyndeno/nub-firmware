#ifndef NETWORK_H
#define NETWORK_H

extern const char *TAG;

void wifi_init_softap(void);
void udp_server_task(void *);

#endif
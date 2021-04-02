#ifndef COMMS_H
#define COMMS_H

#define BUF_SIZE 1024 // change this as appropriate

void UART_init(void);
void UART_rx_task (void *pvParameters);
void UART_tx_task (void *pvParameters);
#endif
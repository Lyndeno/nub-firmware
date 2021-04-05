#ifndef COMMS_H
#define COMMS_H

#define BUF_SIZE (1024) // change this as appropriate

/**
 * Initialize UART and install driver for use. Baud rate and other UART config is
 * defined in function. Start UART tasks after initialization.
 */
void UART_init(void);

/**
 * Task: Handle received bytes from UART buffer and queue them up for routing.
 */
void UART_rx_task (void *pvParameters);

/**
 * Task: Handle message frames from queue and send them to UART TX.
 */
void UART_tx_task (void *pvParameters);
#endif
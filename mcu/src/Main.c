#include "io.h"

#include <stdio.h>
#include <stdlib.h>

#include "comms.h"
#include "buffer.h"
#include "routing.h"


// TODO: Match UART to physical implementation, UART0 and UART1

// IDEA: wifi and transceiver send/receive will probably use very similar code, 
// figure out how to pass UART registers to function

void handle_message(circular_buf * );
void check_frame (circular_buf * );

int main (void) {
    // Initialize buffers first as UART needs them
    init_buffer(&buff_wifi_rx, BUFFER_LENGTH_BYTES);
    UART_WiFi_init(UBRR_WIFI);
    DDR(PORT_STATUS_LED) |= (1<<PIN_STATUS_LED);
    DDR(PORT_TEST_LED) |= (1<<PIN_TEST_LED);

    while (1) {
        PORT(PORT_STATUS_LED) ^= (1<<PIN_STATUS_LED);

        check_frame(&buff_wifi_rx);
        
    }
}
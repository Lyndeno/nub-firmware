#include "buffer.h"

circular_buf buff_wifi_rx;

uint8_t read_buffer(circular_buf *buffer) {
    while(buffer->head == buffer->tail); // we don't want to read something that isn't written yet
    if (buffer->head >= buffer->max) {
        buffer->head = 0;
    }
    uint8_t buff_temp = buffer->buff[buffer->head];
    buffer->buff[buffer->head++] = '\0'; // TODO: This might be a bad idea
    buffer->free++;
    return buff_temp;
}

uint8_t check_buffer(circular_buf *buffer) {
    while(buffer->head == buffer->tail); // make sure we aren't reading data that isn't ready
    if (buffer->head >= buffer->max) {
        buffer->head = 0;
    }
    return buffer->buff[buffer->head];
}

void init_buffer(circular_buf *buffer) {
    buffer->head = 0;
    buffer->tail = 0;
    buffer->max = BUFFER_LENGTH_BYTES;
    buffer->buff = (uint8_t *)malloc(buffer->max * sizeof(uint8_t));
    buffer->free = buffer->max;
}
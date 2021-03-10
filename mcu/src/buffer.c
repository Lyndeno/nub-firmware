#include "buffer.h"

circular_buf buffer0;

uint8_t read_buffer(circular_buf *buffer) {
    //while(check_buffer() == '\0');
    if (buffer->head >= buffer->max) {
        buffer->head = 0;
    }
    uint8_t buff_temp = buffer->buff[buffer->head];
    buffer->buff[buffer->head++] = '\0'; // TODO: This might be a bad idea
    return buff_temp;
}

uint8_t check_buffer(circular_buf *buffer) {
    if (buffer->head >= buffer->max) {
        buffer->head = 0;
    }
    return buffer->buff[buffer->head];
}
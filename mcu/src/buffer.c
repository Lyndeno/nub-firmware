#include "buffer.h"

circular_buf buff_wifi_rx;

/** 
 * First check the head and tail of the buffer and make sure they do not match to avoid
 * reading a byte that does not exist in memory yet. Check if pointer is at the end of the buffer and reset it if so.
 * Byte at location in buffer is stored in a variable, then that location in the buffer is nulled. We then increment
 * the head pointer and return the byte.
 */ 
uint8_t read_buffer(circular_buf *buffer) {
    while(buffer->head == buffer->tail); // we don't want to read something that isn't written yet
    if (buffer->head >= buffer->size) { // check if pointer is out of bounds
        buffer->head = 0; // set pointer to beginning
    }
    uint8_t buff_temp = buffer->buff[buffer->head]; // Store byte in temporary variable before we wipe it
    buffer->buff[buffer->head++] = '\0'; // Clear byte in buffer. TODO: This might be a bad idea and probably won't be needed
    buffer->free++; // Byte is read, thus it is free for overwriting
    return buff_temp; // return byte that was there
}

/** 
 * First check the head and tail of the buffer and make sure they do not match to avoid
 * reading a byte that does not exist in memory yet. Check if pointer is at the end of the buffer and reset it if so.
 * Return the byte.
 */ 
uint8_t check_buffer(circular_buf *buffer) {
    while(buffer->head == buffer->tail); // make sure we aren't reading data that isn't ready
    if (buffer->head >= buffer->size) { // check if pointer is out of bounds
        buffer->head = 0; // check if pointer is out of bounds
    }
    return buffer->buff[buffer->head]; // Return value of byte at pointer
}

/** 
 * Initialize head and tail index to the beginning of the buffer. Allocate memory to the buffer according to given size.
 */ 
void init_buffer(circular_buf *buffer, size_t size) {
    buffer->head = 0; // head is our read pointer
    buffer->tail = 0; // tail is our write pointer
    buffer->size = size; // set maximum amount of bytes for buffer
    buffer->buff = (uint8_t *)malloc(buffer->size * sizeof(uint8_t)); // allocate memory to buffer
    buffer->free = buffer->size; // set free byte count TODO: find a use for free count or remove it
}
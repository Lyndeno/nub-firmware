#ifndef BUFFER_H
#define BUFFER_H

#define FRAME_LENGTH_MAX 256
#define BUFFER_LENGTH_FRAMES 4 // number of frames
#define BUFFER_LENGTH_BYTES (BUFFER_LENGTH_FRAMES * FRAME_LENGTH_MAX)

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/**
 * A circular buffer for reading and writing bytes without shifting the whole array. Separate pointers are used
 * for reading and writing.
 */
typedef struct circular_buf {
    volatile uint8_t * buff; /** Pointer to memory where buffer data is stored */
    size_t head; /** Index to location in buffer to read from */
    volatile size_t tail; /** Index to next location on buffer to write too. */
    size_t max; /** Maximum size of buffer */
    volatile size_t free; /** Free bytes left in buffer */
    bool full; /** Is the buffer full? TODO: This is probably not needed */
} circular_buf;

extern circular_buf buff_wifi_rx;

/** 
 * Read a byte from a buffer as referenced by the head pointer. The byte in the buffer is then cleared before the head pointer is incremented.
 * @param buffer is a pointer to the buffer to be read.
 * @return byte from given buffer.
 */
uint8_t read_buffer(circular_buf * );

/** 
 * Read a byte from a buffer as referenced by the head pointer. This function does not modify the buffer or increment the head pointer.
 * @param buffer is a pointer to the buffer to be read.
 * @return byte from given buffer.
 */
uint8_t check_buffer(circular_buf * );

/** 
 * Initializes a given buffer for use. Head and tail pointers are set to the beginning and memory is allocated for the buffer.
 * @param buffer is a pointer to the buffer to be initialized
 */
void init_buffer(circular_buf * );

#endif
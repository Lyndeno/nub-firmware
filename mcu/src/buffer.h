#ifndef BUFFER_H
#define BUFFER_H

#define FRAME_LENGTH_MAX 256
#define BUFFER_LENGTH_FRAMES 4 // number of frames
#define BUFFER_LENGTH_BYTES (BUFFER_LENGTH_FRAMES * FRAME_LENGTH_MAX)

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>



typedef struct circular_buf {
    volatile uint8_t * buff;
    size_t head;
    volatile size_t tail;
    size_t max;
    bool full;
} circular_buf;

extern circular_buf buffer0;

uint8_t read_buffer(circular_buf * );
uint8_t check_buffer(circular_buf * );

#endif
#include "buffer.h"
#include "comms.h"

// Kyle Stuff

#define RX0_BUFFER_SIZE 256
#define RX1_BUFFER_SIZE 256


unsigned char rx0Buffer[RX0_BUFFER_SIZE];
unsigned char rx0ReadPos = 0;
unsigned char rx0WritePos = 0;
unsigned char rx0BufferPointer = 0;

unsigned char rx1Buffer[RX1_BUFFER_SIZE];
unsigned char rx1ReadPos = 0;
unsigned char rx1WritePos = 0;
unsigned char rx1BufferPointer = 0;


uint16_t unread0Bytes = 0;
uint16_t unread1Bytes = 0;

//

circular_buf buff_wifi_rx;
circular_buf buff_trans_rx;

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

// Looking at next char
uint8_t peekChar(uart_dev UARTPort) {
	uint8_t ret = '\0';
	
	if (UARTPort == Transceiver) {
		if (buff_trans_rx.head != buff_trans_rx.tail) {
			ret = buff_trans_rx.buff[buff_trans_rx.head];
		}
	}
	else if (UARTPort == WiFi) {
		if (buff_wifi_rx.head != buff_wifi_rx.tail) {
			ret = buff_wifi_rx.buff[buff_wifi_rx.head];
		}
	}
	return ret;
}


// Getting each unsigned character
// TODO: Replace this with read_buffer
unsigned char getChar(uart_dev UARTPort) {
	unsigned char ret = '\0';
	
	if ( UARTPort == Transceiver ) {
		
		if (buff_trans_rx.head != buff_trans_rx.tail) {
			//unread0Bytes--;
			buff_trans_rx.free++;
			ret = buff_trans_rx.buff[buff_trans_rx.head++];
			//rx0ReadPos ++;

			if (buff_trans_rx.head >= buff_trans_rx.size) {
				buff_trans_rx.head = 0;
			}
		}
	}
	else if ( UARTPort == WiFi ) {
		
		if (buff_wifi_rx.head != buff_wifi_rx.tail) {
			//unread1Bytes--;
			buff_wifi_rx.free++;
			ret = buff_wifi_rx.buff[buff_wifi_rx.head++];
			//rx1ReadPos ++;

			if (buff_wifi_rx.head >= buff_wifi_rx.size) {
				buff_wifi_rx.head = 0;
			}
		}
	}
	
	return ret;
}

// Skipping to the most current point in the buffer
void skipBuffer(uart_dev UARTPort) {
	
	if(UARTPort == Transceiver) {
		//unread0Bytes = 0;
		buff_trans_rx.free = buff_trans_rx.size;
		buff_trans_rx.head = buff_trans_rx.tail;
	}
	else if (UARTPort == WiFi) {
		//unread1Bytes = 0;
		buff_wifi_rx.free = buff_wifi_rx.size;
		buff_wifi_rx.head = buff_wifi_rx.tail;
	}
	
}

// Allows the program to jump through the rx0 (transceiver) buffer 
uint8_t parseBufferForVal(int movePtr, uint16_t jmpToPos) {
	
	uint8_t returnChar;
	// No jump to specific address, 
	if (jmpToPos == 0) {
		if (buff_trans_rx.head + movePtr > buff_trans_rx.size){
			buff_trans_rx.head = buff_trans_rx.head + movePtr - buff_trans_rx.size;
		}
		else {
			buff_trans_rx.head= buff_trans_rx.head + movePtr;
		}
		
		returnChar = buff_trans_rx.buff[buff_trans_rx.head];
		if (movePtr > 0) {
			//unread0Bytes = unread0Bytes - movePtr;
			buff_trans_rx.free = buff_trans_rx.free + movePtr;
		}
		
	}
	
	else {
		
		// Jump to specific address then move read position 
		if (jmpToPos < buff_trans_rx.head) {
			//unread0Bytes = RX0_BUFFER_SIZE - buff_trans_rx.head + jmpToPos;
			buff_trans_rx.free = buff_trans_rx.head - jmpToPos;
		}
		else {
			//unread0Bytes = jmpToPos - buff_trans_rx.head;
			buff_trans_rx.free = buff_trans_rx.size - jmpToPos + buff_trans_rx.head;
		}
		
		buff_trans_rx.head = jmpToPos;
		
		if (buff_trans_rx.head + movePtr > buff_trans_rx.size) {
			buff_trans_rx.head = buff_trans_rx.head + movePtr - buff_trans_rx.size;
		}
		else {
			buff_trans_rx.head = buff_trans_rx.head + movePtr;
		}
		
		
		returnChar = buff_trans_rx.buff[buff_trans_rx.head];
		
		if (movePtr > 0) {
			//unread0Bytes = unread0Bytes - movePtr;
			buff_trans_rx.free = buff_trans_rx.size + movePtr;
		}
	}
	
	return returnChar;
	
}


uint16_t  parseBufferForPtr(int movePtr) {
	
	
	if (buff_trans_rx.head + movePtr > buff_trans_rx.size) {
		return buff_trans_rx.head + movePtr - buff_trans_rx.size;
	}
	else {
		return buff_trans_rx.head + movePtr;
	}
	
}
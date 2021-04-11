#ifndef HANDLEUART_H
#define  HANDLEUART_H

void TXWrite(unsigned char c[], uint8_t size, uint16_t uartPort );

uint8_t peekChar(void);

unsigned char getChar(uint8_t UARTPort);

void skipBuffer(void);

uint8_t parseBufferForVal(int movePointer);
uint16_t BytesUnRead();
uint16_t parseBufferForPtr(int movePtr);

uint8_t uartReady();

#endif
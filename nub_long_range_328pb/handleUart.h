#ifndef HANDLEUART_H
#define  HANDLEUART_H

void TXWrite(unsigned char c[], uint8_t size, uint16_t uartPort );

uint8_t peekChar(void);

unsigned char getChar(uint8_t UARTPort);

void skipBuffer(uint8_t uartPort);


uint8_t Bytes0UnRead();
uint8_t Bytes1UnRead();


uint8_t uartReady();

#endif
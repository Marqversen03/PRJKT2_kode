#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define START_BYTE 0xAA
#define END_BYTE 0xBB
#define Data_SIZE 64

extern volatile uint8_t UART_Started;
extern volatile uint8_t Index;
extern volatile uint8_t Data_Array[Data_SIZE];
extern volatile uint8_t FrameReady;
extern volatile uint8_t FrameLength;
extern volatile uint8_t OverflowFlag;

void UART0_Init(void);
void UART0_RST(void);
void SendChar(char c);
void SendString(const char* str);

#endif

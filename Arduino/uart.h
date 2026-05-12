#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define START_BYTE 0xAA
#define END_BYTE   0xBB

#define BUFFER_SIZE 10

extern volatile uint8_t Data_Array[BUFFER_SIZE];
extern volatile uint8_t FrameReady;

void UART0_Init(void);
void SendChar(char c);
void SendString(const char* str);
void SendTemp(void);

#endif
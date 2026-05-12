#define F_CPU 16000000UL
#include "uart.h"

volatile uint8_t UART_Started = 0;
volatile uint8_t Index = 0;
volatile uint8_t Data_Array[Data_SIZE] = {0};
volatile uint8_t FrameReady = 0;
volatile uint8_t FrameLength = 0;
volatile uint8_t OverflowFlag = 0;

void UART0_RST(void)
{
	UART_Started = 0;
	Index = 0;
	OverflowFlag = 0;
}

void UART0_Init(void)
{
	UBRR0H = 0;
	UBRR0L = 103;
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	sei();
}

void SendChar(char c)
{
	while (!(UCSR0A & (1 << UDRE0)))
	{
	}
	UDR0 = c;
}

void SendString(const char* str)
{
	while (*str)
	{
		SendChar(*str++);
	}
}

ISR(USART0_RX_vect)
{
	uint8_t data = UDR0;

	if (!UART_Started)
	{
		if (data == START_BYTE)
		{
			UART_Started = 1;
			Index = 0;
			OverflowFlag = 0;
		}
		return;
	}

	if (data == START_BYTE)
	{
		Index = 0;
		OverflowFlag = 0;
		return;
	}

	if (data == END_BYTE)
	{
		FrameLength = Index;
		FrameReady = 1;
		UART_Started = 0;
		Index = 0;
		return;
	}

	if (Index < Data_SIZE)
	{
		Data_Array[Index] = data;
		Index++;
	}
	else
	{
		OverflowFlag = 1;
		UART_Started = 0;
		Index = 0;
	}
}
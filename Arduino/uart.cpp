#define F_CPU 16000000UL
#include "uart.h"
#include <stdlib.h>

// UART state
volatile uint8_t Data_Array[BUFFER_SIZE];
volatile uint8_t FrameReady = 0;

static uint8_t index = 0;
static uint8_t Uart_avail = 0;

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
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void SendString(const char* str)
{
    while (*str)
    {
        SendChar(*str++);
    }
}

void SendTemp(void)
{
    int temp = (rand() % 17) + 5;

    SendChar(START_BYTE);
    SendChar('T');
    SendChar(temp);
    SendChar(END_BYTE);
}

// ---------------- RECEIVE (ISR) ----------------
ISR(USART0_RX_vect)
{
    uint8_t data = UDR0;

    if (!Uart_avail)
    {
        if (data == START_BYTE)
        {
            Uart_avail = 1;
            index = 0;
        }
        return;
    }

    if (data == END_BYTE)
    {
        FrameReady = 1;
        Uart_avail = 0;
        return;
    }

    if (index < BUFFER_SIZE)
    {
        Data_Array[index++] = data;
    }
    else
    {
        Uart_avail = 0; //overflow
    }
}

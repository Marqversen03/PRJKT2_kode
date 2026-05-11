#include <avr/io.h>
#include <avr/interrupt.h>
#include "Initial_Timer_5.h"

// Global variables
volatile uint16_t seconds = 0;
volatile uint8_t fem_min = 0;
volatile uint8_t femten_min = 0;

volatile uint8_t Check_temp = 0;
volatile uint8_t Check_Pos = 0;

// ---------------- INIT ----------------
void timer5_init(void)
{
    TCCR5B |= (1 << WGM52);   // CTC mode
    OCR5A = 15624;            // 1 second (16MHz / 1024)

    TCCR5B |= (1 << CS52) | (1 << CS50); // prescaler 1024
    TIMSK5 |= (1 << OCIE5A); // enable interrupt

    sei();
}

// ---------------- ISR ----------------
ISR(TIMER5_COMPA_vect)
{
    seconds++;

    if (seconds % 300 == 0)
    {
        Check_temp = 1;
    }

    if (seconds % 900 == 0)
    {
        Check_Pos = 1;
    }
}
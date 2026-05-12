/*
 * interrupt 5 min.c
 *
 * Created: 07/05/2026 12.21.38
 * Author : josef
 */ 

//Timer hver 5 min.

#include <avr/io.h> // 16 bit muligt ift. timers
#include <avr/interrupt.h>
#include "Initial_Timer_5.h"

volatile uint16_t seconds = 0;
volatile uint8_t Auto = 0;
volatile uint8_t Check_Pos = 0;

void timer5_init() {
	// CTC mode (Clear Timer on Compare Match)
	TCCR5B |= (1 << WGM52);

	// Prescaler = 1024
	TCCR5B |= (1 << CS52) | (1 << CS50);

	// Compare value for 1 second
	OCR5A = 15624;

	// Enable Timer5 compare interrupt
	TIMSK5 |= (1 << OCIE5A);

	// Enable global interrupts
	sei();
}

// Interrupt handler (kører hver 1 sekund)
ISR(TIMER5_COMPA_vect) {
	seconds++;

	if (seconds >= 5) {  // 5 minutter. Vil loope indtil 300 overflows.
		Check_temp = 1;
		Check_Pos += 1;
		seconds = 0; // Variable reset - hvis du vil gentage

	}
}


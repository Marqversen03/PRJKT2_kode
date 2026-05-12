#include "Varmelampe.h"

// LED PIN PE4, PE5, PE3, PG5

void pwmInit()
{
	// Set pins as OUTPUT
	DDRE |= (1 << PE3) | (1 << PE4) | (1 << PE5);
	DDRG |= (1 << PG5);

	// Timer3 -> D2, D3, D5
	TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1) | (1 << WGM30);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	// Timer0 -> D4
	TCCR0A = (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
	TCCR0B = (1 << CS01) | (1 << CS00);
}




// Fast PWM, 8-bit, prescaler 64
void setLed(uint8_t pin, uint8_t level)
{
	if (pin == 1) OCR3B = level;   // D2
	if (pin == 2) OCR3C = level;   // D3
	if (pin == 3) OCR0B = level;   // D4
	if (pin == 4) OCR3A = level;   // D5
}


uint8_t int_to_state(uint8_t state)
{
	switch (state)
	{
		case 1: return OFF;
		case 2: return LOW;
		case 3: return MID;
		case 4: return HIGH;
		default: return OFF;
	}
}

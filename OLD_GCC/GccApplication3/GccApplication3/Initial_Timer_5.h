/*
 * Initial_Timer_5
 *
 * Created: 07/05/2026 14.12.21
 *  Author: josef
 */ 


#ifndef TIMER5_H_
#define TIMER5_H_

#include <avr/io.h>

// Global variabel
extern volatile uint16_t seconds;
extern volatile uint8_t Check_temp;
extern volatile uint8_t Check_Pos;

// Initialiseringsfunktion
void timer5_init();

#endif /* TIMER5_H_ */

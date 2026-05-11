#ifndef TIMER5_H_
#define TIMER5_H_

#include <avr/io.h>

// Time tracking
extern volatile uint16_t seconds;
extern volatile uint8_t Check_temp;
extern volatile uint8_t Check_Pos;

// Init
void timer5_init(void);

#endif
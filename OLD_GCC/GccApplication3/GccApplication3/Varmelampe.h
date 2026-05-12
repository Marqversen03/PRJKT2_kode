#ifndef VARMELAMPE_H
#define VARMELAMPE_H

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>

#define OFF  0
#define LOW  64
#define MID  128
#define HIGH 255

void pwmInit();
void setLed(uint8_t pin, uint8_t level);
uint8_t int_to_state(uint8_t state);

#endif
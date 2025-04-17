#ifndef GLOBAL_H
#define GLOBAL_H
#include "Library/library.h"

extern volatile uint16_t joystick_x_value;
extern volatile uint16_t joystick_y_value;

void joystick_interrupt_handler();
void setup_interrupt();
//
void setup_joystick_interrupt();


extern bool system_state;
extern volatile bool bomba_ligada;

#endif
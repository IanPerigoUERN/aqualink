#ifndef MATRIX_H
#define MATRIX_H

#include "Library/library.h"


// Definição do número de LEDs e pino.

#define LED_PIN 7    // GPIO conectado no primeiro LED
#define NUM_LEDS 25   // Número de LEDs na linha

void matrix_control();
void set_pixel_color(uint32_t color);
#endif
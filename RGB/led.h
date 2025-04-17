
#ifndef LED_H
#define LED_H

#include "Library/library.h"

#define LED_RED_PIN   13
#define LED_GREEN_PIN 11

// Constantes de tempo em microsegundos
#define TIME_ABOVE_75  (5ULL * 1000000ULL)
#define TIME_ABOVE_50  (10ULL * 1000000ULL)
#define TIME_BELOW_50  (5ULL * 1000000ULL)

void led_rgb_init(void);
void led_rgb_update(uint8_t percent);

bool led_is_red(void);

#endif

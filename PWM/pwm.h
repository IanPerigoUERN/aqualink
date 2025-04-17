#ifndef PWM_H
#define PWM_H

#include "Library/library.h"

//PWM-Joystick-Led
#define DIVIDER_PWM 16.0
#define PERIOD 4086



// Constantes PWM Buzzer
#define DIVISOR_CLK_PWM 16.0
#define PERIOD_BUZZER 2000
#define LED_STEP 32 // Incremento para a matriz de led simulando intensidade da placa solar
#define MAX_WRAP_DIV_BUZZER 16
#define MIN_WRAP_DIV_BUZZER 2

#endif
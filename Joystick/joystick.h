#ifndef JOYSTICH_H
#define JOYSTICH_H

#include "Library/library.h"

#define JOYSTICK_X_PIN 26
#define JOYSTICK_Y_PIN 27
#define SW 22
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1

#define MAX_ADC 4095.0f
#define MAX_HEIGHT_POCO 10.0f  // Altura máxima em metros
#define AREA_POCO 1.0f        // Área da base em m²

// Parâmetros da caixa d'água (exemplo)
#define MAX_HEIGHT_CAIXA 2.0f // Altura máxima em metros
#define AREA_CAIXA 0.5f       // Área da base em m²

// Inicializa o joystick

void setup_joystick();
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);
void update_poco_level();

void joystick_led_function();
void acionar_bomba(bool ligar);

#endif
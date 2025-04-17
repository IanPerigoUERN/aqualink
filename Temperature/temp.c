/*

#include "temp.h"
//static const float CONVERSION_FACTOR = 3.3f / (1 << 12);

float temperature_update(void) {
    adc_select_input(sensor_temp);
    // Realiza a leitura do ADC
    uint16_t adc_value = adc_read();  // Leitura do ADC de 16 bits
    float volt = (3.3f / 4095) * adc_value;
    float temperature = 27.0f - (volt - 0.706f) / 0.001721f;

    //printf("Temperatura: %.2f °C\n", temperature);
    return temperature;
}

*/
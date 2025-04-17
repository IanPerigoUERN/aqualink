
#include "led.h"
// Variáveis globais estáticas para controle do tempo e estado do LED
bool is_red = true; // Estado atual: true = vermelho, false = verde

volatile uint8_t thingspeak_batt_state = 0; 

static uint64_t above_threshold_start = 0;
static uint64_t below_threshold_start = 0;
void led_rgb_init(void) {
    // Configura os pinos como saída
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);

    // Inicializa com LED vermelho aceso
    gpio_put(LED_RED_PIN, true);
    gpio_put(LED_GREEN_PIN, false);
    is_red = true;
    thingspeak_batt_state = 0; 
    
    above_threshold_start = 0;
    below_threshold_start = 0;
}

void led_rgb_update(uint8_t percent) {
    uint64_t now = time_us_64();

    if (is_red) {
        // Enquanto estiver em estado vermelho, verificar se o PWM satisfaz as condições para mudar para verde
        //printf("RGB: Bateria Descarregada\n");
        if (percent > 75) {
            if (above_threshold_start == 0) {
                above_threshold_start = now;
            } else if ((now - above_threshold_start) >= TIME_ABOVE_75) {
                // Altera de vermelho para verde
                gpio_put(LED_RED_PIN, false);
                gpio_put(LED_GREEN_PIN, true);
                is_red = false;
                thingspeak_batt_state = 1;
                //printf("RGB: Bateria carregada\n");
                above_threshold_start = 0;
                below_threshold_start = 0;
            }
        } else if (percent > 50 && percent <= 75) {
            if (above_threshold_start == 0) {
                above_threshold_start = now;
            } else if ((now - above_threshold_start) >= TIME_ABOVE_50) {
                // Altera de vermelho para verde
                gpio_put(LED_RED_PIN, false);
                gpio_put(LED_GREEN_PIN, true);
                is_red = false;
                thingspeak_batt_state = 1; 
                //printf("RGB: Bateria carregada\n");
                above_threshold_start = 0;
                below_threshold_start = 0;
            }
        } else {
            // Se não cumprir condições, reseta a contagem
            above_threshold_start = 0;
        }
    } else {
        // Estado atualmente verde: se o PWM estiver abaixo de 50%, contar para mudar de volta para vermelho
        if (percent < 50) {
            if (below_threshold_start == 0) {
                below_threshold_start = now;
            } else if ((now - below_threshold_start) >= TIME_BELOW_50) {
                // Altera de verde para vermelho
                gpio_put(LED_RED_PIN, true);
                gpio_put(LED_GREEN_PIN, false);
                is_red = true;
                thingspeak_batt_state = 0;
                //printf("RGB: Bateria Descarregada\n");
                below_threshold_start = 0;
                above_threshold_start = 0;
            }
        } else {
            below_threshold_start = 0;
        }
    }
}

bool led_is_red(void) {
    return is_red;
}
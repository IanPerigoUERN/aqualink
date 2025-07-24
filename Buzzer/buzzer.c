
#include "buzzer.h"


volatile bool bomba_ligada; // Variável global para o estado da bomba

void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_wrap(slice_num, 8000);
    pwm_set_gpio_level(pin, 0); // Desliga o PWM inicialmente
}

void beep(uint pin, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin, 2048);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

void update_buzzer(volatile bool bomba_ligada, bool sys_state) {
    if (bomba_ligada && sys_state){
    //  gpio_put(BUZZER_PIN, true); // Liga o buzzer
    pwm_set_gpio_level(BUZZER_PIN, 2048);
    bomba_ligada = 1;
        //printf("BUZZER: Bomba ligada\n");
    }
    else{
    //  gpio_put(BUZZER_PIN, false); // Desliga o buzzer
    pwm_set_gpio_level(BUZZER_PIN, 0);
    bomba_ligada = 0;
        //printf("BUZZER: Bomba desligada\n");
    }
}


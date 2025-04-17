
#include "matriz.h"

uint16_t matrix_led_level = 0; // Inicialização dos níveis de PWM para os LEDs
uint slice_led_matrix;    

volatile uint8_t thingspeak_percent = 0;

void setup_pwm_matrix(uint led, uint *slice, uint16_t level)
{
  gpio_set_function(led, GPIO_FUNC_PWM); // Configura o pino do LED como saída PWM
  *slice = pwm_gpio_to_slice_num(led);   // Obtém o slice do PWM associado ao pino do LED
  pwm_set_clkdiv(*slice, DIVIDER_PWM);   // Define o divisor de clock do PWM
  pwm_set_wrap(*slice, PERIOD);          // Configura o valor máximo do contador (período do PWM)
  pwm_set_gpio_level(led, level);        // Define o nível inicial do PWM para o LED
  pwm_set_enabled(*slice, true);         // Habilita o PWM no slice correspondente ao LED
}

void led_function(){
    // Inicializa o PWM para o LED da matriz (LED_B)
    setup_pwm_matrix(LED_PIN, &slice_led_matrix, matrix_led_level);
    
    // Configura os botões: A (pino 5) para aumentar a intensidade e B (pino 6) para diminuir
    gpio_init(BUTTON_PIN_A);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);
    
    gpio_init(BUTTON_PIN_B);
    gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_B);

    while (true) {
        // Se o botão A for pressionado (lógica ativa low) e houver margem para aumentar
        if (!gpio_get(BUTTON_PIN_A) && matrix_led_level <= (PERIOD - LED_STEP)) {
            matrix_led_level += LED_STEP;
            sleep_ms(50);  // Debounce
        }
        // Se o botão B for pressionado e houver margem para diminuir
        if (!gpio_get(BUTTON_PIN_B) && matrix_led_level >= LED_STEP) {
            matrix_led_level -= LED_STEP;
            sleep_ms(50);  // Debounce
        }
        pwm_set_gpio_level(LED_PIN, matrix_led_level);
        sleep_ms(100);
    }
}

void ws2818_init(PIO pio, int sm, uint pin) {
    uint offset = pio_add_program(pio, &ws2818_program);
    ws2818_program_init(pio, sm, offset, pin, 800000, false);
}

// Função para enviar cor para o NeoPixel
void set_pixel_color(uint32_t color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        pio_sm_put_blocking(pio0, 0, color << 8);  // Envia a cor para todos os LEDs
    }
}

uint32_t rgb_to_grb(uint8_t r, uint8_t g, uint8_t b) {
    return ((g << 16) | (r << 8) | b);  // Formato GRB usado pelo WS2812
}

void set_matrix() {
    stdio_init_all();  // Inicializa comunicação UART
    PIO pio = pio0;
    int sm = 0;  // State machine 0
    ws2818_init(pio, sm, LED_PIN);

    while (true) {
       
        set_pixel_color(rgb_to_grb(0, 255, 0));  // Verde
        
    }

    
}

//ol system_state = false; // true: sistema ativo

static void evaluate_system_state(uint8_t percent) {
    // Se a matriz estiver acima de 75% e o RGB estiver verde, ativa o sistema.
    if (percent > 75 && !led_is_red()) {
        system_state = true;
    }
    // Se a matriz estiver abaixo de 50% ou se o LED RGB estiver vermelho, desativa o sistema.
    else if (percent < 50 || led_is_red()) {
        system_state = false;
    }
    
    // Debug: imprime o estado
    //printf("system_state: %s\n", system_state ? "true" : "false");
    //sleep_ms(1000);
}

void matrix_control() {
    stdio_init_all();  // Inicializa a comunicação UART, se necessário
    PIO pio = pio0;
    int sm = 0;        // Utiliza a state machine 0
    ws2818_init(pio, sm, LED_PIN);
    
    // Inicializa os botões para controle de intensidade
    gpio_init(BUTTON_PIN_A);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);
    
    gpio_init(BUTTON_PIN_B);
    gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_B);
    
    // Variável para indicar se o sistema pode ligar
    bool can_power = false;
    led_rgb_init();
    
    while (true) {
       
        // Botão A: aumenta intensidade (garante que não passe de 255)
        if (!gpio_get(BUTTON_PIN_A) && matrix_led_level <= (255 - LED_STEP)) {
            matrix_led_level += LED_STEP;
            sleep_ms(50); // debounce
        }
        // Botão B: diminui intensidade (garante que não fique abaixo de 0)
        if (!gpio_get(BUTTON_PIN_B) && matrix_led_level >= LED_STEP) {
            matrix_led_level -= LED_STEP;
            sleep_ms(50); // debounce
        }
        
        // Calcula a porcentagem da intensidade
        uint8_t percent = (matrix_led_level * 100) / 255;
        
        thingspeak_percent = percent;
        // Verifica a potência dos painéis solares simulada
        if (percent > 75) {
            can_power = true;
            //printf("PLACA SOLAR:>75\n");
            //sleep_ms(1000);
        } else if (percent >= 50) {
            can_power = true;
            //printf("PLACA SOLAR >=50\n");
            //sleep_ms(1000);
        } else {
            can_power = false;
            //printf("PLACA SOLAR <50\n");
            //sleep_ms(1000);
        }
        
        // Atualiza a cor: usamos o valor atual no canal verde
        uint32_t color = rgb_to_grb(0, matrix_led_level, 0);
        set_pixel_color(color);
        
        
        led_rgb_update(percent);
        evaluate_system_state(percent);
        sleep_ms(500);
    
    }

}
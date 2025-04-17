#include "global.h"

// Variáveis globais para armazenar os valores do joystick
volatile uint16_t joystick_x_value = 0;
volatile uint16_t joystick_y_value = 0;

// Variável auxiliar para alternar a leitura entre eixo X e eixo Y
static bool read_x = true;

// Handler de interrupção do ADC
void adc_irq_handler() {
    // Enquanto houver dados no FIFO, realiza a leitura
    while (adc_fifo_get_level() > 0) {
        uint16_t data = adc_read();
        if (read_x) {
            // Leitura do eixo X (canal 1)
            joystick_x_value = data;
            // Prepara a próxima conversão para o eixo Y (canal 0)
            adc_select_input(ADC_CHANNEL_0);
        } else {
            // Leitura do eixo Y (canal 0)
            joystick_y_value = data;
            // Prepara a próxima conversão para o eixo X (canal 1)
            adc_select_input(ADC_CHANNEL_1);
        }
        read_x = !read_x;
    }
}

// Função para configurar o ADC com interrupção para o joystick
void setup_joystick_interrupt() {
    // Inicializa o ADC
    adc_init();

    // Configura os pinos usados para os eixos (já inicializados em joystick.c, se necessário)
    adc_gpio_init(JOYSTICK_X_PIN); // Pino do eixo X
    adc_gpio_init(JOYSTICK_Y_PIN); // Pino do eixo Y

    // Configura o FIFO do ADC: ativado, com disparo após 1 amostra, sem DMA
    adc_fifo_setup(true,    // habilita o FIFO
                   true,    // habilita o disparo ao preencher o FIFO
                   1,       // nível de disparo
                   false,   // não roda em modo DREQ
                   false);  // não inverte os dados

    // Configura o handler de interrupção para o FIFO do ADC
    irq_set_exclusive_handler(ADC_IRQ_FIFO, adc_irq_handler);
    irq_set_enabled(ADC_IRQ_FIFO, true);
    adc_irq_set_enabled(true);

    // Inicia a conversão: seleciona inicialmente o canal X (canal 1) e executa o ADC
    adc_select_input(ADC_CHANNEL_1);
    adc_run(true);
}

bool system_state = false;
volatile bool bomba_ligada = false;
//volatile bool pump_state = false;
// RGB

// Matriz

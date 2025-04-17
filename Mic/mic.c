/*

#include "Library/library.h"
#include "mic.h"


#define MIC_CHANNEL 2
#define MIC_PIN (26 + MIC_CHANNEL)
#define ADC_CLOCK_DIV 96.f
#define SAMPLES 200
#define ADC_ADJUST(x) (x * 3.3f / (1 << 12u) - 1.65f)
#define ADC_STEP (3.3f/5.f)
#define SOUND_THRESHOLD 3
#define abs(x) ((x < 0) ? (-x) : (x))

extern void update_buzzer(bool pump_state, bool sys_state);
extern void set_pixel_color(uint32_t color);
extern void gpio_put(uint pin, bool value);


// DMA e buffer
uint dma_channel;
dma_channel_config dma_cfg;
uint16_t adc_buffer[SAMPLES];

void sample_mic() {
    adc_fifo_drain();
    adc_run(false);
    dma_channel_configure(dma_channel, &dma_cfg,
        adc_buffer,
        &(adc_hw->fifo),
        SAMPLES,
        true
    );
    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_channel);
    adc_run(false);
}

float mic_power() {
    float avg = 0.f;
    for (uint i = 0; i < SAMPLES; ++i)
        avg += adc_buffer[i] * adc_buffer[i];

    avg /= SAMPLES;
    return sqrt(avg);
}

uint8_t get_intensity(float v) {
    uint count = 0;
    while ((v -= ADC_STEP/20) > 0.f)
        ++count;
    return count;
}

// Executa uma única iteração da atualização (replacing o loop infinito original)
void mic_update() {
    // Realiza a amostragem e processamento
    sample_mic();
    float avg = mic_power();
    avg = 2.f * abs(ADC_ADJUST(avg));

    uint intensity = get_intensity(avg);

    // Print para debug
    printf("[Debug] Tensão média: %.3f V, Intensidade: %d\n", avg, intensity);

    // Verifica se deve desligar os periféricos
    if (intensity > SOUND_THRESHOLD) {
        update_buzzer(false, false);        // Desliga o buzzer.
        set_pixel_color(0);                   // Limpa a matriz de LEDs.
        gpio_put(LED_RED_PIN, false);         // Desliga o LED RGB (vermelho).
        gpio_put(LED_GREEN_PIN, false);       // Desliga o LED RGB (verde).
        printf("Intensidade alta (%d) - Dispositivos desligados\n", intensity);
    } else {
        printf("Intensidade OK: %d\n", intensity);
    }
}

*/
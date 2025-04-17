#include "joystick.h"
volatile float poco_volume = 2.0f;
volatile float caixa_volume = 0.5f;


void setup_joystick()
{
  // Inicializa o ADC e os pinos de entrada analógica
  adc_init();         // Inicializa o módulo ADC
  adc_gpio_init(JOYSTICK_X_PIN); // Configura o pino VRX (eixo X) para entrada ADC
  adc_gpio_init(JOYSTICK_Y_PIN); // Configura o pino VRY (eixo Y) para entrada ADC

  // Inicializa o pino do botão do joystick
  gpio_init(SW);             // Inicializa o pino do botão
  gpio_set_dir(SW, GPIO_IN); // Configura o pino do botão como entrada
  gpio_pull_up(SW);          // Ativa o pull-up no pino do botão para evitar flutuações
}

void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
  // Leitura do valor do eixo X do joystick
  adc_select_input(ADC_CHANNEL_1); // Seleciona o canal ADC para o eixo X
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vrx_value = adc_read();         // Lê o valor do eixo X (0-4095)

  // Leitura do valor do eixo Y do joystick
  adc_select_input(ADC_CHANNEL_0); // Seleciona o canal ADC para o eixo Y
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vry_value = adc_read();         // Lê o valor do eixo Y (0-4095)
}

void update_poco_level() {
  setup_joystick();
  uint16_t vrx_value, vry_value;
  bool agua_poco;           
  bool caixa_agua_cheia;    // true se a caixa d'água está cheia
  bool caixa_agua_vazia;    // true se a caixa d'água está vazia

  
      joystick_read_axis(&vrx_value, &vry_value);
      float norm_poço = (float)vrx_value / MAX_ADC;
      float norm_caixa = (float)vry_value / MAX_ADC;

        poco_volume = norm_poço * MAX_HEIGHT_POCO * AREA_POCO;
        caixa_volume = norm_caixa * MAX_HEIGHT_CAIXA * AREA_CAIXA;
        
    
        //printf("Volume do poço: %.2f m³\n", poco_volume);
        //printf("Volume da caixa d'água: %.2f m³\n", caixa_volume);
     
   
      // Atualiza os estados com base nas leituras atuais
      agua_poco = (vrx_value >= 30);
      caixa_agua_cheia = (vry_value >= 2400);
      caixa_agua_vazia = (vry_value <= 500);

      if (caixa_agua_cheia && bomba_ligada) {
       
          bomba_ligada = false;
          
      }
      // Lógica para ligar a bomba: somente se houver água no poço, a caixa estiver vazia e a bomba não estiver ligada
      else if (agua_poco && caixa_agua_vazia && !bomba_ligada) {
        
          bomba_ligada = true;
          
      }
      // Desliga a bomba se não houver água suficiente no poço
      else if (!agua_poco && bomba_ligada) {
      
          bomba_ligada = false;
          
      }

      // Atualiza o estado do buzzer com base no estado da bomba e do sistema
      update_buzzer(bomba_ligada, system_state);

      sleep_ms(5000);
  }

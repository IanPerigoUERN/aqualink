#include "Library/library.h"


// Função que será executada no Core1 (matrix_control)
void matrix_thread() {
    matrix_control();
}


int main() {
    stdio_init_all(); // Comunicação serial UART
    
    DisplayInit();
    sleep_ms(4000);
    
    pwm_init_buzzer(BUZZER_PIN);
   
    multicore_launch_core1(matrix_thread);
    
    if(cyw43_arch_init()) { // Inicializa o módulo Wi‑Fi
        printf("Falha ao inicializar o Wi‑Fi\n");
        DisplayFalha();
        update_poco_level();
        return 1;
    }
    
    // Usa a nova função com timeout (20 segundos)
    if (!conexao_wifi(20000)) {
        // Se não conectar, exibe mensagem de falha e o bitmap, mas prossegue com o código
        DisplayFalha();
        sleep_ms(2000);
        DisplayLogo();
    } else {
        DisplayConectando();
        DisplayConectado();
        sleep_ms(4000);
        DisplayLogo();
    }
    
    uint64_t last_thingspeak_update = time_us_64();
    
    while(true) {
        
        update_poco_level(); // Atualiza os níveis do poço e aciona o buzzer
        
        // Tenta enviar para o ThingSpeak a cada 10 segundos (thing só aceita a cada 15 segundos, mas nesse frequencia ficou o mais rápido que consegui)
        if((time_us_64() - last_thingspeak_update) >= 10000000) {
            conectar_thingspeak_service();
            last_thingspeak_update = time_us_64();
        }
        
        sleep_ms(50);
    }
}
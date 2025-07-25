#include "http_server.h"
/*
static struct tcp_pcb *pcb;  // ponteiro para a estrutura de controle de bloco TCP - THING SPEAK


bool erro_dns = false;  // Indica se houve um erro na resolução de DNS
//static bool dnsResolved = false;
*/
//  volatile bool conectado = false;
// volatile bool mqtt_status = false; // Variável global para o status da conexão MQTT


//mqtt_client_t *client = NULL;


// bool conexao_wifi(uint32_t timeout_ms) {// Função para conectar ao Wi-Fi
//     uint64_t start = time_us_64();
//     printf("Conectando ao Wi‑Fi...\n");
//     cyw43_arch_enable_sta_mode();
//     while ((time_us_64() - start) < (timeout_ms * 1000)) {
//     // Apenas uma tentativa com timeout interno da função
//         if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, timeout_ms) == 0) {
//             conectado = true;
//             printf("Wi‑Fi conectado!\n");
//         } else {
//             printf("Falha ao conectar ao Wi‑Fi.\n");
//             conectado = false;
//         }

//         return conectado;
//     }
// }
 
// bool MQTT_BROKER(uint32_t timeout_ms ) {

    
    
//    if (conectado && !mqtt_status) {
    
//     printf("Conectando ao Broker MQTT...\n");
//     mqtt_setup(NOME_DO_DISPOSITIVO, IP_DO_BROKER, USER_DO_BROKER, SENHA_DO_BROKER);
//     sleep_ms(5000);

//         if (mqtt_is_connected()){
//             printf("Já está conectado ao broker MQTT.\n");
//             mqtt_status = true;
//             sleep_ms(2000);
//         } else{
//             printf("Cliente não conectado");
//             sleep_ms(2000);
//             return !mqtt_status;    
//         }

//     }
// }


//    bool MQTT_PUBLISHER() {
//      char payload[256];  // espaço suficiente para o JSON
//      bool sucesso = false;

//     // Monta o JSON com os dados formatados
//     snprintf(payload, sizeof(payload),
//         "{ \"poco_volume\": %.2f, \"caixa_volume\": %.2f, \"Painéis Solares\": %d, \"bateria\": %d, \"bomba_ligada\": %d }",
//         poco_volume,
//         caixa_volume,
//         thingspeak_percent,
//         thingspeak_batt_state,
//         bomba_ligada  
        
//     );
        
//     sucesso = mqtt_comm_publish(CANAL_DO_BROKER, (const uint8_t *)payload, strlen(payload));
//     sleep_ms(5000);    
// }
    





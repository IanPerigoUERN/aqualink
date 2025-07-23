#include "http_server.h"
/*
static struct tcp_pcb *pcb;  // ponteiro para a estrutura de controle de bloco TCP - THING SPEAK


bool erro_dns = false;  // Indica se houve um erro na resolução de DNS
//static bool dnsResolved = false;
*/
volatile bool conectado = false;
volatile bool mqtt_status = false; // Variável global para o status da conexão MQTT

//mqtt_client_t *client = NULL;

bool conexao_wifi(uint32_t timeout_ms) {// Função para conectar ao Wi-Fi
    uint64_t start = time_us_64();
    printf("Conectando ao Wi‑Fi...\n");
    cyw43_arch_enable_sta_mode();
    while ((time_us_64() - start) < (timeout_ms * 1000)) {
    // Apenas uma tentativa com timeout interno da função
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, timeout_ms) == 0) {
            conectado = true;
            printf("Wi‑Fi conectado!\n");
        } else {
            printf("Falha ao conectar ao Wi‑Fi.\n");
            conectado = false;
        }

        return conectado;
    }
}

 
bool MQTT_BROKER(uint32_t timeout_ms ) {
    
   while (conectado && !mqtt_status) {
    printf("Conectando ao Broker MQTT...\n");
    mqtt_setup(NOME_DO_DISPOSITIVO, IP_DO_BROKER, USER_DO_BROKER, SENHA_DO_BROKER);
    sleep_ms(5000);

        if (mqtt_is_connected()){
            printf("Já está conectado ao broker MQTT.\n");
            mqtt_status = true;
            sleep_ms(2000);
        } else{
            printf("Cliente não conectado");
            sleep_ms(2000);
            continue;    
        }

    }
}


   bool MQTT_PUBLISHER() {
     char mensagem[100];
    bool sucesso = false;

    // Converte o float para string antes de publicar
    snprintf(mensagem, sizeof(mensagem), "%.2f", poco_volume);  // por exemplo: "12.34"

    // Verifica se estamos conectados antes de tentar publicar
    while (mqtt_status) {
        sucesso = mqtt_comm_publish(CANAL_DO_BROKER, (const uint8_t *)mensagem, strlen(mensagem));
        sleep_ms(20000);
        if (sucesso) {
            printf("Dados publicados com sucesso!\n");
            return true;
        } else {
            printf("Falha ao publicar dados. Tentando novamente...\n");
            sleep_ms(10000); // Espera 10 segundos antes de tentar novamente
        }
    }
    return false;
}
    
/*
void my_dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *arg) { // Callback chamado quando a resolução de DNS é concluída

    if (ipaddr) {// Verifica se o endereço IP foi resolvido com sucesso
        printf("Endereço IP do ThingSpeak: %s\n", ipaddr_ntoa(ipaddr));
        // Cria uma nova conexão TCP
        pcb = tcp_new();
        if (!pcb) {
            printf("Erro ao criar conexão TCP\n");
            return;
        }
        erro_dns = false;  // Indica que a resolução de DNS foi bem-sucedida
        // Conecta ao servidor ThingSpeak
        tcp_connect(pcb, ipaddr, THINGSPEAK_PORT, callback_conexao_tcp);
    } else {
        printf("Erro na resolução de DNS\n");
        erro_dns = true; // Indica que houve um erro na resolução de DNS
    }
}
*/
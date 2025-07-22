#include "http_server.h"

static struct tcp_pcb *pcb;  // ponteiro para a estrutura de controle de bloco TCP - THING SPEAK


bool erro_dns = false;  // Indica se houve um erro na resolução de DNS
static bool dnsResolved = false;
//static ip_addr_t IP_DO_BROKER;
/*
err_t callback_envio_tcp(void *arg, struct tcp_pcb *tpcb, u16_t len) { // Callback chamado quando os dados são enviados com sucesso
    printf("Dados enviados com sucesso!\n");
    return ERR_OK;
}

err_t callback_conexao_tcp(void *arg, struct tcp_pcb *tpcb, err_t err) { // Callback chamado quando a conexão TCP é estabelecida com sucesso

    if (err == ERR_OK) {
        printf("Conectado ao servidor!\n");
        
        // Cria a requisição HTTP GET para enviar dados ao ThingSpeak
        char request[200];
        snprintf(request, sizeof(request),
                 "GET /update?api_key=%s&field1=%f&field2=%f&field3=%hhu&field4=%hhu&field5=%hhu HTTP/1.1\r\n"
                 "Host: api.thingspeak.com\r\n"
                 "Connection: close\r\n\r\n",
                 API_KEY, poco_volume,caixa_volume,thingspeak_percent,thingspeak_batt_state, bomba_ligada);

        // Envia a requisição HTTP
        tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
        tcp_output(tpcb);
        tcp_sent(tpcb, callback_envio_tcp);
    } else {
        printf("Falha ao conectar ao servidor\n");
    }
    return ERR_OK;
}
*/
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

bool conexao_wifi(uint32_t timeout_ms) {// Função para conectar ao Wi-Fi
    
    uint64_t start = time_us_64();
    bool conectado = false;
    
    printf("Conectando ao Wi‑Fi...\n");
    cyw43_arch_enable_sta_mode();
    
    // Tenta conectar enquanto não exceder o tempo limite
    while ((time_us_64() - start) < (timeout_ms * 1000)) {
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID,WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000) == 0) {
            conectado = true;
            mqtt_setup(NOME_DO_DISPOSITIVO, IP_DO_BROKER, USER_DO_BROKER, SENHA_DO_BROKER);
            break;
        }
        printf("Falha ao conectar ao Wi‑Fi, tentando novamente...\n");
        sleep_ms(5000);
    }
    
    if (conectado) {
        printf("Wi‑Fi conectado!\n");
    }
    
    return conectado;
}






/*

// ...existing code...
static ip_addr_t broker_ip_addr;
void my_dns_callback_wrapper(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    if (ipaddr) {
        broker_ip_addr = *ipaddr;  // Usando o novo nome
        dnsResolved = true;
        printf("DNS resolvido: %s -> %s\n", name, ipaddr_ntoa(ipaddr));
    } else {
        printf("Falha na resolução DNS para %s\n", name);
        dnsResolved = false;
    }
}

void conectar_thingspeak_service(void) {
    printf("Iniciando conexão com o Broker...\n");

    ip_addr_t server_ip;
    err_t err = dns_gethostbyname(DNS_BROKER, &server_ip, my_dns_callback_wrapper, NULL);

    if (err == ERR_OK) {
        // Resolução imediata
        my_dns_callback_wrapper(DNS_BROKER, &server_ip, NULL);
    } else if (err != ERR_INPROGRESS) {
        printf("Erro ao iniciar a resolução DNS: %d\n", err);
        return;
    }

    // Aguarda a resolução do DNS
    while (!dnsResolved) {
        sleep_ms(100);
    }

    // Converte ip_addr_t para string e conecta ao Broker MQTT
    char broker_ip_str[16];
    snprintf(broker_ip_str, sizeof(broker_ip_str), "%s", ipaddr_ntoa(&broker_ip_addr));
    mqtt_setup(NOME_DO_DISPOSITIVO, broker_ip_str, USER_DO_BROKER, SENHA_DO_BROKER);
}
*/
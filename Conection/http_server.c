#include "http_server.h"

static struct tcp_pcb *pcb;  // ponteiro para a estrutura de controle de bloco TCP - THING SPEAK


bool erro_dns = false;  // Indica se houve um erro na resolução de DNS
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
        if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000) == 0) {
            conectado = true;
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


static bool dnsResolved = false;  // Flag para indicar que o DNS foi resolvido

// Função wrapper para a callback, que atualiza a flag e chama a callback original
static void my_dns_callback_wrapper(const char *name, const ip_addr_t *ipaddr, void *arg) {
    dnsResolved = true;  // Indica que a resolução foi concluída
    my_dns_found_callback(name, ipaddr, arg);  // Chama a callback original
}

// Função que realiza a conexão via Wi-Fi e a resolução DNS para o ThingSpeak
void conectar_thingspeak_service(void) {
    printf("Iniciando conexão com o ThingSpeak...\n");

    // Conecta ao Wi-Fi
    //conexao_wifi();

    // Inicia a resolução DNS para o hostname do ThingSpeak
    ip_addr_t server_ip;
    err_t err = dns_gethostbyname(THINGSPEAK_HOST, &server_ip, my_dns_callback_wrapper, NULL);

    if (err == ERR_OK) {
        // Resolução imediata; chama a callback manualmente
        my_dns_callback_wrapper(THINGSPEAK_HOST, &server_ip, NULL);
    } else if (err != ERR_INPROGRESS) {
        printf("Erro ao iniciar a resolução DNS: %d\n", err);
        return;
    }

  
}


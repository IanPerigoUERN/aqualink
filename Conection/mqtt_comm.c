#include "mqtt_comm.h"



// Preencha isso antes de chamar

/* Variável global estática para armazenar a instância do cliente MQTT
 * 'static' limita o escopo deste arquivo */
static mqtt_client_t *client;

//bool connected = false;

/* Callback de conexão MQTT - chamado quando o status da conexão muda
 * Parâmetros:
 *   - client: instância do cliente MQTT
 *   - arg: argumento opcional (não usado aqui)
 *   - status: resultado da tentativa de conexão */
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    bool *connected = (bool*)arg;
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("MQTT conectado com sucesso!\n");
        *connected = true;
    } else {
        printf("MQTT falha na conexão: %d\n", status);
        *connected = false;
    }
}

/* Função para configurar e iniciar a conexão MQTT
 * Parâmetros:
 *   - client_id: identificador único para este cliente
 *   - broker_ip: endereço IP do broker como string (ex: "192.168.1.1")
 *   - user: nome de usuário para autenticação (pode ser NULL)
 *   - pass: senha para autenticação (pode ser NULL) */
void mqtt_setup(const char *client_id, const char *broker_ip, const char *user, const char *pass) {
    ip_addr_t broker_addr;  // Estrutura para armazenar o IP do broker
    
    // Converte o IP de string para formato numérico
    if (!ip4addr_aton(broker_ip, &broker_addr)) {
        printf("Erro no IP\n");
        return;
    }

    // Cria uma nova instância do cliente MQTT
    client = mqtt_client_new();
    if (client == NULL) {
        printf("Falha ao criar o cliente MQTT\n");
        return;
    }

    // Configura as informações de conexão do cliente
    struct mqtt_connect_client_info_t ci = {
        .client_id = client_id,  // ID do cliente
        .client_user = user,     // Usuário (opcional)
        .client_pass = pass,      // Senha (opcional)
        // .cliente_info.keep_alive = 60, // Tempo de keep-alive em segundos
        // .cliente_info.clean_session = true
    };
 // Armazena as informações de conexão
    // Inicia a conexão com o broker
    // Parâmetros:
    //   - client: instância do cliente
    //   - &broker_addr: endereço do broker
    //   - 1883: porta padrão MQTT
    //   - mqtt_connection_cb: callback de status
    //   - NULL: argumento opcional para o callback
    //   - &ci: informações de conexão
    mqtt_client_connect(client, &broker_addr, 1883, mqtt_connection_cb, NULL, &ci);
}

/* Callback de confirmação de publicação
 * Chamado quando o broker confirma recebimento da mensagem (para QoS > 0)
 * Parâmetros:
 *   - arg: argumento opcional
 *   - result: código de resultado da operação */
static void mqtt_pub_request_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("Publicação MQTT enviada com sucesso!\n");
    } else {
        printf("Erro ao publicar via MQTT: %d\n", result);
    }
}

/* Função para publicar dados em um tópico MQTT
 * Parâmetros:
 *   - topic: nome do tópico (ex: "sensor/temperatura")
 *   - data: payload da mensagem (bytes)
 *   - len: tamanho do payload */
bool mqtt_comm_publish(const char *topic, const uint8_t *data, size_t len) {
    err_t status = mqtt_publish(
        client,
        topic,
        data,
        len,
        0,  // QoS 0
        0,  // Não reter
        mqtt_pub_request_cb,
        NULL
    );

    if (status != ERR_OK) {
        printf("[MQTT] mqtt_publish falhou: %d\n", status);
        return false;
    }

    return true;
}

bool mqtt_is_connected(){
    if (mqtt_client_is_connected(client)) return true;
    else return false;
}



///////////////Resolução de DNS e conexão com o broker MQTT

volatile bool conectado = false;
volatile bool mqtt_status = false; // Variável global para o status da conexão MQTT
bool dns_resolvido = false;

const char* HOSTNAME_BROKER = "broker.iot.natal.br";

static struct mqtt_connect_client_info_t client_info;



void dns_found_cb(const char *name, const ip_addr_t *ipaddr, void *arg) { // Callback chamado quando a resolução de DNS é concluída

    if (ipaddr) {// Verifica se o endereço IP foi resolvido com sucesso
        printf("Endereço IP do Broker: %s\n", ipaddr_ntoa(ipaddr));

            if (ipaddr != NULL) {
                printf("DNS resolvido para %s: %s\n", name, ipaddr_ntoa(ipaddr));
                mqtt_client_connect(client, ipaddr, MQTT_PORT, mqtt_connection_cb, NULL, &client_info);
            } 
            else {
            printf("Erro na resolução de DNS\n");
            erro_dns = true; // Indica que houve um erro na resolução de DNS
        }
    }
}




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

ip_addr_t resolved_ip;

void resolve_and_connect(const char *hostname) {
    //ip_addr_t resolved_ip;

    err_t err = dns_gethostbyname(hostname, &resolved_ip, dns_found_cb, NULL);
    printf("IP resolvido: %s\n", ipaddr_ntoa(&resolved_ip));

    if (err == ERR_OK) {
        // IP já estava em cache, pode usar diretamente
        printf("Hostname resolvido instantaneamente: %s\n", ipaddr_ntoa(&resolved_ip));
        //mqtt_client_connect(client, &resolved_ip, MQTT_PORT, mqtt_connection_cb, NULL, &client_info);
        sleep_ms(3000);
    } else if (err == ERR_INPROGRESS) {
        // DNS assíncrono, aguardará dns_found_cb
        printf("Resolução DNS em andamento...\n");
    } else {
        printf("Erro na solicitação DNS: %d\n", err);
    }
}
bool MQTT_BROKER(uint32_t timeout_ms ) {

    const char *ip_str = ipaddr_ntoa(&resolved_ip);
    
   if (conectado && !mqtt_status) {
    
    printf("Conectando ao Broker MQTT...\n");
    mqtt_setup(NOME_DO_DISPOSITIVO, ip_str, USER_DO_BROKER, SENHA_DO_BROKER);
    sleep_ms(5000);

        if (mqtt_is_connected()){
            printf("Já está conectado ao broker MQTT.\n");
            mqtt_status = true;
            sleep_ms(2000);
        } else{
            printf("Cliente não conectado");
            sleep_ms(2000);
            return !mqtt_status;    
        }

    }
}

   bool MQTT_PUBLISHER() {
     char payload[256];  // espaço suficiente para o JSON
     bool sucesso = false;

    // Monta o JSON com os dados formatados
    snprintf(payload, sizeof(payload),
        "{ \"poco_volume\": %.2f, \"caixa_volume\": %.2f, \"Painéis Solares\": %d, \"bateria\": %d, \"bomba_ligada\": %d }",
        poco_volume,
        caixa_volume,
        thingspeak_percent,
        thingspeak_batt_state,
        bomba_ligada  
        
    );
        
    sucesso = mqtt_comm_publish(CANAL_DO_BROKER, (const uint8_t *)payload, strlen(payload));
    sleep_ms(5000);    
}
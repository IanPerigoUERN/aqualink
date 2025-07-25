
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "Library/library.h"


// Definindo o pino do LED

#define API_KEY "HHN5DZPDF459VX1E" // Substitua pela sua chave de API do ThingSpeak
#define THINGSPEAK_HOST "api.thingspeak.com"
#define THINGSPEAK_PORT 80

extern bool erro_dns;

//static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
//static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err);

err_t callback_envio_tcp(void *arg, struct tcp_pcb *tpcb, u16_t len);
err_t callback_conexao_tcp(void *arg, struct tcp_pcb *tpcb, err_t err);
void my_dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *arg);
void conectar_thingspeak_service(void);
void thingspeak_update_loop(void);





 bool conexao_wifi();
 bool MQTT_BROKER();

bool MQTT_PUBLISHER();





///Variáveis passadas para o thingspeak
extern volatile float poco_volume;
extern volatile float caixa_volume;
extern volatile uint8_t thingspeak_percent;
extern volatile uint8_t thingspeak_batt_state;
extern volatile uint8_t thingspeak_pump_state;
extern volatile bool bomba_ligada; // Variável global para o estado da bomba

#endif // HTTP_SERVER_H

#ifndef MQTT_COMM_H
#define MQTT_COMM_H

#include "Library/library.h"

void mqtt_setup(const char *client_id, const char *broker_ip, const char *user, const char *pass);
bool mqtt_comm_publish(const char *topic, const uint8_t *data, size_t len);
bool mqtt_is_connected();

// void dns_found_cb(const char *name, const ip_addr_t *ipaddr, void *arg);
// void resolve_and_connect(const char *hostname);
bool MQTT_BROKER();
bool conexao_wifi();
void resolve_and_connect();

#endif
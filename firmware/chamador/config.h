#pragma once

// Rede Wi-Fi (o ESP32 só funciona em redes de 2,4 GHz)
#define WIFI_NOME      "iPhone de Matheus"
#define WIFI_SENHA     "matheus17"

// Broker MQTT público da HiveMQ: funciona pela internet, sem instalar nada
#define MQTT_SERVIDOR  "broker.hivemq.com"
#define MQTT_PORTA     1883

// O broker público não pede usuário nem senha
#define MQTT_USUARIO   ""
#define MQTT_SENHA     ""

// Começo de todos os tópicos. No broker público QUALQUER pessoa pode
// usar o mesmo nome, por isso usamos um nome bem único.
// Tem que ser igual ao topicoBase do painel (dashboard/config.js).
#define TOPICO_BASE    "catolica-n3-time7/senha"

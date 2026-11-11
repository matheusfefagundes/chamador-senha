// ===============================================================
//  CONFIGURAÇÃO DE EXEMPLO  (pode ir para o GitHub)
//
//  Como usar na Arduino IDE:
//  1. Copie todo o conteúdo desta aba
//  2. Crie uma aba nova (Ctrl+Shift+N) com o nome  config.h  e cole
//  3. No config.h, coloque o nome e a senha reais do Wi-Fi
//  4. NUNCA envie o config.h para o GitHub (ele tem a senha do Wi-Fi)
// ===============================================================
#pragma once

// Rede Wi-Fi (o ESP32 só funciona em redes de 2,4 GHz)
// Dica: use o roteador do celular de alguém da equipe
#define WIFI_NOME      "nome-da-sua-rede"
#define WIFI_SENHA     "senha-da-sua-rede"

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

// ===============================================================
//  CHAMADOR DE SENHA IoT  -  ESP32 + MQTT
//
//  - Botão físico: a pessoa "pega uma senha" (entra na fila)
//  - Painel web manda "proxima" ou "repetir" pelo broker MQTT
//  - Display de 7 segmentos mostra o último dígito da senha chamada
//  - Buzzer apita a cada chamada
//  - Se o Wi-Fi ou o broker cair, ele tenta reconectar sozinho
//    e o botão continua funcionando enquanto isso
//
//  Bibliotecas: PubSubClient (Nick O'Leary) + placa "esp32" da Espressif
// ===============================================================
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"   // copie o config.example.h e renomeie para config.h

// ---------------- Pinos ----------------
const int PINO_BOTAO    = 4;    // botão "pegar senha" (outro lado no GND)
const int PINO_BUZZER   = 18;
const int PINO_LED_REDE = 2;    // LED azul da placa: aceso = conectado ao broker

// Segmentos do display, na ordem:  a,  b,  c,  d,  e,  f,  g
// Todos ficam no MESMO lado do ESP32 (o lado do D23), porque a placa é larga
// e só sobra furo livre na protoboard de um dos lados.
// Na placa, o 17 aparece escrito como TX2 e o 16 como RX2.
const int PINOS_DISPLAY[7]   = { 23, 22, 21, 19, 17, 16, 5 };

// Se o display acender "ao contrário" (tudo aceso e o número apagado),
// troque para true. Isso depende do modelo: catodo comum ou anodo comum.
const bool DISPLAY_ANODO_COMUM = false;

// Quais segmentos acendem em cada número (bit 0 = a ... bit 6 = g)
const byte DESENHO_NUMEROS[10] = {
  0b0111111,  // 0
  0b0000110,  // 1
  0b1011011,  // 2
  0b1001111,  // 3
  0b1100110,  // 4
  0b1101101,  // 5
  0b1111101,  // 6
  0b0000111,  // 7
  0b1111111,  // 8
  0b1101111   // 9
};
const byte DESENHO_TRACO = 0b1000000;  // "-" = nenhuma senha chamada ainda

// ---------------- Tópicos MQTT ----------------
const String TOPICO_FILA    = String(TOPICO_BASE) + "/fila";     // ESP32 -> painel
const String TOPICO_ATUAL   = String(TOPICO_BASE) + "/atual";    // ESP32 -> painel
const String TOPICO_AVISO   = String(TOPICO_BASE) + "/aviso";    // ESP32 -> painel
const String TOPICO_STATUS  = String(TOPICO_BASE) + "/status";   // ESP32 -> painel
const String TOPICO_COMANDO = String(TOPICO_BASE) + "/comando";  // painel -> ESP32

// ---------------- Estado do sistema ----------------
int senhaEmitida = 0;   // última senha que alguém pegou no botão
int senhaChamada = 0;   // última senha chamada pelo atendente (0 = nenhuma)

WiFiClient   wifiCliente;
PubSubClient mqtt(wifiCliente);

unsigned long ultimaTentativaWifi = 0;
unsigned long ultimaTentativaMqtt = 0;
unsigned long ultimoApertoBotao   = 0;
int estadoAnteriorBotao = HIGH;

// Quantas pessoas estão esperando
int tamanhoDaFila() {
  return senhaEmitida - senhaChamada;
}

// =============== DISPLAY E BUZZER ===============
void desenharNoDisplay(byte desenho) {
  for (int i = 0; i < 7; i++) {
    bool acender = bitRead(desenho, i);
    if (DISPLAY_ANODO_COMUM) acender = !acender;
    digitalWrite(PINOS_DISPLAY[i], acender ? HIGH : LOW);
  }
}

void mostrarSenhaNoDisplay() {
  if (senhaChamada == 0) {
    desenharNoDisplay(DESENHO_TRACO);
  } else {
    desenharNoDisplay(DESENHO_NUMEROS[senhaChamada % 10]);  // último dígito
  }
}

void bipCurto() {
  tone(PINO_BUZZER, 1500);
  delay(80);
  noTone(PINO_BUZZER);
}

void tocarChamada() {  // "bip-bip" de chamada
  for (int i = 0; i < 2; i++) {
    tone(PINO_BUZZER, 1000);
    delay(200);
    noTone(PINO_BUZZER);
    delay(120);
  }
}

// =============== PUBLICAR NO BROKER ===============
// "true" no final = mensagem retida: quem abrir o painel depois já recebe o valor
void publicarFila() {
  if (mqtt.connected()) mqtt.publish(TOPICO_FILA.c_str(), String(tamanhoDaFila()).c_str(), true);
}

void publicarSenhaAtual() {
  if (mqtt.connected()) mqtt.publish(TOPICO_ATUAL.c_str(), String(senhaChamada).c_str(), true);
}

void publicarAviso(String texto) {
  Serial.println("Aviso: " + texto);
  if (mqtt.connected()) mqtt.publish(TOPICO_AVISO.c_str(), texto.c_str());
}

// =============== COMANDOS QUE CHEGAM DO PAINEL ===============
void chamarProxima() {
  if (tamanhoDaFila() <= 0) {
    publicarAviso("Ninguém na fila");
    return;
  }
  senhaChamada++;
  mostrarSenhaNoDisplay();  // 1º muda o atuador...
  tocarChamada();
  publicarSenhaAtual();     // ...depois confirma o novo estado para o painel
  publicarFila();
  publicarAviso("Senha " + String(senhaChamada) + " chamada");
}

void repetirChamada() {
  if (senhaChamada == 0) {
    publicarAviso("Nenhuma senha foi chamada ainda");
    return;
  }
  tocarChamada();
  publicarAviso("Senha " + String(senhaChamada) + " chamada de novo");
}

// Esta função roda sozinha toda vez que chega uma mensagem do broker
void aoReceberMensagem(char* topico, byte* dados, unsigned int tamanho) {
  String mensagem = "";
  for (unsigned int i = 0; i < tamanho; i++) mensagem += (char)dados[i];
  mensagem.trim();
  Serial.println("Comando recebido: " + mensagem);

  if (mensagem == "proxima")      chamarProxima();
  else if (mensagem == "repetir") repetirChamada();
  else publicarAviso("Comando desconhecido: " + mensagem);
}

// =============== CONEXÃO COM O BROKER ===============
void conectarMqtt() {
  Serial.print("Conectando ao broker MQTT... ");
  String idCliente = "chamador-" + String((uint32_t)ESP.getEfuseMac(), HEX);

  // Usuário e senha vazios viram NULL (broker sem login)
  const char* usuario = strlen(MQTT_USUARIO) > 0 ? MQTT_USUARIO : NULL;
  const char* senha   = strlen(MQTT_SENHA)   > 0 ? MQTT_SENHA   : NULL;

  // "Testamento" (Last Will): se o ESP32 sumir, o BROKER publica "offline" no lugar dele
  bool conectou = mqtt.connect(idCliente.c_str(), usuario, senha,
                               TOPICO_STATUS.c_str(), 1, true, "offline");
  if (conectou) {
    Serial.println("conectado!");
    mqtt.publish(TOPICO_STATUS.c_str(), "online", true);
    mqtt.subscribe(TOPICO_COMANDO.c_str());
    // Ao (re)conectar, manda o estado atual. Assim o painel fica em dia
    // mesmo que a fila tenha mudado enquanto a internet estava fora.
    publicarFila();
    publicarSenhaAtual();
  } else {
    Serial.print("falhou, código ");
    Serial.println(mqtt.state());
  }
}

// Chamada em todo loop. Nunca trava: tenta de novo a cada alguns segundos.
void cuidarDaConexao() {
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(PINO_LED_REDE, LOW);
    if (millis() - ultimaTentativaWifi > 10000) {
      ultimaTentativaWifi = millis();
      Serial.println("Sem Wi-Fi. Tentando reconectar...");
      WiFi.disconnect();
      WiFi.begin(WIFI_NOME, WIFI_SENHA);
    }
    return;
  }

  if (!mqtt.connected()) {
    digitalWrite(PINO_LED_REDE, LOW);
    if (millis() - ultimaTentativaMqtt > 5000) {
      ultimaTentativaMqtt = millis();
      conectarMqtt();
    }
    return;
  }

  digitalWrite(PINO_LED_REDE, HIGH);
  mqtt.loop();  // recebe mensagens e mantém a conexão viva
}

// =============== BOTÃO "PEGAR SENHA" ===============
void lerBotao() {
  int estadoAtual = digitalRead(PINO_BOTAO);
  // Detecta o momento do aperto (de solto para apertado),
  // com 250 ms de intervalo para ignorar o "tremido" do botão
  if (estadoAtual == LOW && estadoAnteriorBotao == HIGH &&
      millis() - ultimoApertoBotao > 250) {
    ultimoApertoBotao = millis();
    senhaEmitida++;
    bipCurto();
    Serial.println("Senha " + String(senhaEmitida) + " retirada. Fila: " + String(tamanhoDaFila()));
    publicarFila();  // se estiver sem internet, envia quando reconectar
  }
  estadoAnteriorBotao = estadoAtual;
}

// =============== INÍCIO ===============
void setup() {
  Serial.begin(115200);

  pinMode(PINO_BOTAO, INPUT_PULLUP);  // resistor interno: solto = HIGH, apertado = LOW
  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(PINO_LED_REDE, OUTPUT);
  for (int i = 0; i < 7; i++) pinMode(PINOS_DISPLAY[i], OUTPUT);
  mostrarSenhaNoDisplay();  // começa mostrando "-"

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NOME, WIFI_SENHA);
  ultimaTentativaWifi = millis();

  mqtt.setServer(MQTT_SERVIDOR, MQTT_PORTA);
  mqtt.setCallback(aoReceberMensagem);
}

void loop() {
  cuidarDaConexao();
  lerBotao();
}

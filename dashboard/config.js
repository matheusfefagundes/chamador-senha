// CONFIGURAÇÃO DO PAINEL
// Não tem nenhuma senha aqui (o broker público não pede login),
// por isso este arquivo pode ir para o GitHub normalmente.
window.CONFIG = {
  // Broker público da HiveMQ, pela porta segura de WebSocket (wss).
  // Precisa ser wss:// porque o painel fica num site https (GitHub Pages).
  broker: "wss://broker.hivemq.com:8884/mqtt",

  // O MESMO TOPICO_BASE usado no config.h do ESP32
  topicoBase: "catolica-n3-time7/senha",

  // O broker público não pede login: deixe vazio
  usuario: "",
  senha: ""
};

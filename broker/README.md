# Broker MQTT

Escolhemos o broker público da HiveMQ: `broker.hivemq.com`.

**Por quê:** os computadores da faculdade não permitem instalar programas,
então não dava para rodar o Mosquitto. O broker da HiveMQ funciona pela
internet, é gratuito, não precisa de cadastro e aceita as duas formas de
conexão que o projeto precisa:

| Quem conecta | Endereço | Porta |
|---|---|---|
| ESP32 (MQTT normal) | broker.hivemq.com | 1883 |
| Painel no navegador (WebSocket seguro) | wss://broker.hivemq.com:8884/mqtt | 8884 |

**Cuidados:** o broker é público. Qualquer pessoa que souber o nome dos
tópicos consegue ler as mensagens e mandar comandos. Por isso usamos um
nome de tópico único e não mandamos nenhum dado pessoal.
Ele serve para estudo e testes, não para um sistema de verdade.

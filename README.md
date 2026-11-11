# Chamador de senha IoT
ESP32 + broker MQTT público (HiveMQ) + painel web. Nada precisa ser instalado
além da Arduino IDE.
Equipe: Nome 1, Nome 2, Nome 3, Nome 4

## Você vai precisar
- ESP32 (30 pinos), display de 7 segmentos, 7 resistores de 300 Ω, botão, buzzer e protoboard
- 10 jumpers fêmea-macho (ESP32 → protoboard) e 4 jumpers macho-macho
- Arduino IDE (para gravar o ESP32) e um navegador (para o painel)
- Uma rede Wi-Fi de 2,4 GHz com internet (o roteador de um celular serve)

## 1. Monte o circuito
O ESP32 fica fora da protoboard e se liga às peças por jumpers fêmea-macho.
Siga docs/protoboard.png (furo por furo) e docs/circuito.png (fio por fio).
- Segmentos a-g nos pinos D23, D22, D21, D19, TX2 (17), RX2 (16) e D5,
  cada um passando por um resistor de 300 Ω
- COM do display no GND
- Botão entre o D4 e o GND
- Buzzer: perna maior no D18, perna menor no GND

## 2. Broker
Não precisa instalar nada: usamos o broker público broker.hivemq.com
(porta 1883 para o ESP32 e wss porta 8884 para o painel). Veja broker/README.md.

## 3. Grave o ESP32 pela Arduino IDE
1. Na Arduino IDE 2, instale a placa "esp32 by Espressif Systems"
   e a biblioteca "PubSubClient" (Nick O'Leary).
2. Abra firmware/chamador/chamador.ino (Arquivo > Abrir).
3. Crie uma aba nova chamada config.h (Ctrl+Shift+N), cole o conteúdo
   de config.example.h e coloque o nome e a senha do Wi-Fi.
4. Placa: ESP32 Dev Module. Porta: a COM do ESP32. Clique em Carregar.
5. No Monitor Serial (115200), confira a mensagem "conectado!".

## 4. Abra o painel
Acesse https://USUARIO.github.io/chamador-senha/dashboard/
(o TOPICO_BASE do config.h e o topicoBase do dashboard/config.js precisam ser iguais:
catolica-n3-time7/senha).

## 5. Use
Aperte o botão para pegar uma senha. No painel, clique em "Chamar próxima senha".

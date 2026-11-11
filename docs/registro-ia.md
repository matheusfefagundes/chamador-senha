# Registro do uso de inteligência artificial

Ferramenta: Claude (Anthropic).

| Data | Para que | Pedido feito | O que aceitamos | O que rejeitamos | O que mudamos depois |
|---|---|---|---|---|---|
| 03/11 | Escolher o uso do sistema | “Preciso de ajuda para fazer esse projeto, me dê ideias do que posso fazer, que seja simples mas criativo.” | A lista de ideias. Escolhemos primeiro o medidor de barulho. | – | Depois de informar o kit que temos, trocamos de ideia (linha seguinte). |
| 03/11 | Adaptar a ideia ao material da equipe | “Para sua informação, eu tenho um Kit Iniciante V8 para Arduino da RoboCore.” | O aviso de que o kit não traz ESP32 nem sensor de som. | O medidor de barulho, porque exigiria comprar um sensor de som. | Passamos a usar só peças do kit e o nosso ESP32. |
| 03/11 | Novas ideias com as peças do kit | “Quais outros projetos posso fazer com isso?” | O chamador de senha (botão como fonte de dados; display e buzzer como atuadores). | As outras ideias (estufa, cofre, vaga de garagem etc.). | – |
| 03/11 | Tópicos MQTT, código do ESP32, dashboard e guia de estudo | “Quero que você faça um PDF mostrando o passo a passo completo de todo o projeto (…) a interface bonita que eu quis dizer também é a da senha, faça com um bom CSS.” | Tópicos MQTT, código do ESP32 com reconexão e Last Will, dashboard em HTML/CSS/MQTT.js e o guia em PDF. | completar | completar: o que a equipe alterou no código ou no visual |
| 04/11 | Gravar pela Arduino IDE | “Lembrando que é pelo Arduino IDE.” | Passo a passo de instalação da placa esp32 e da biblioteca PubSubClient, e criação do config.h como aba. | – | – |
| 04/11 | Alternativa ao Mosquitto | “O computador da minha faculdade não permite baixar coisas, então não consigo baixar o Mosquitto, me dê alguma alternativa pela web.” | Broker público da HiveMQ, dashboard no GitHub Pages e teste pelo cliente web da HiveMQ. | O Mosquitto local (a faculdade bloqueia instalação) e criar um broker privado no HiveMQ Cloud (desnecessário para o trabalho). | Usamos o nome de tópico catolica-n3-time7 para não misturar com outras equipes. |
| 04/11 | Testar o broker | “Onde eu vou?” (com print da página da HiveMQ) | O caminho para o cliente web da HiveMQ e como conectar (porta 8884 com SSL). | – | – |
| 04/11 | Entender o circuito | “Me dê uma imagem de como deve ficar o circuito, não consegui entender pelo guia, o que são os segmentos.” / “E a protoboard?” | As imagens explicando os segmentos e as ligações. | – | – |
| 04/11 | Ajustar ao nosso ESP32 | “O meu ESP vai da letra b até a j (…) no meu não sobra.” | Mudar todos os pinos usados para o lado do D23 da placa. | – | Trocamos os pinos do display no código para 23, 22, 21, 19, 17, 16 e 5. |
| 11/11 | Ajustar ao nosso buzzer | “O nosso buzzer ocupa mais espaço (…) faça de uma maneira que o ESP não fique na protoboard.” | Montagem com o ESP32 fora da protoboard, ligado por jumpers fêmea-macho. | – | completar |
| 11/11 | Organizar a entrega | “Me mande tudo que já foi feito de acordo com as entregas que o professor solicitou.” | A organização destes PDFs. | – | completar: o que a equipe revisou ou mudou |

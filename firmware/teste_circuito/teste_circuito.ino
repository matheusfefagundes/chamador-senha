// ===============================================================
//  TESTE DO CIRCUITO (sem Wi-Fi e sem MQTT)
//  Serve só para conferir se os fios estão certos.
//
//  O que deve acontecer:
//  1. Um bip curto ao ligar
//  2. Os segmentos acendem um por vez: a, b, c, d, e, f, g
//     (o Monitor Serial mostra qual deveria estar aceso)
//  3. O display conta de 0 a 9 sem parar
//  4. Ao apertar o botão: bip e mensagem no Monitor Serial
// ===============================================================

// Mesmos pinos do programa principal (a, b, c, d, e, f, g)
const int PINOS_DISPLAY[7] = { 23, 22, 21, 19, 17, 16, 5 };
const int PINO_BUZZER = 18;
const int PINO_BOTAO  = 4;

// Se tudo acender ao contrário (o número apagado e o resto aceso), troque para true
const bool DISPLAY_ANODO_COMUM = false;

const byte NUMEROS[10] = {
  0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110,
  0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111
};

void desenhar(byte desenho) {
  for (int i = 0; i < 7; i++) {
    bool acender = bitRead(desenho, i);
    if (DISPLAY_ANODO_COMUM) acender = !acender;
    digitalWrite(PINOS_DISPLAY[i], acender ? HIGH : LOW);
  }
}

void bip() {
  tone(PINO_BUZZER, 1500);
  delay(120);
  noTone(PINO_BUZZER);
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 7; i++) pinMode(PINOS_DISPLAY[i], OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  delay(500);

  Serial.println("Teste do buzzer: deve dar um bip agora");
  bip();

  const char letras[] = "abcdefg";
  for (int i = 0; i < 7; i++) {
    Serial.print("Agora deve acender SÓ o segmento ");
    Serial.println(letras[i]);
    desenhar(1 << i);
    delay(1200);
  }
  Serial.println("Agora o display conta de 0 a 9. Aperte o botão para testar.");
}

void loop() {
  static int numero = 0;
  static unsigned long ultimaTroca = 0;

  if (millis() - ultimaTroca > 700) {
    ultimaTroca = millis();
    desenhar(NUMEROS[numero]);
    numero = (numero + 1) % 10;
  }

  if (digitalRead(PINO_BOTAO) == LOW) {
    Serial.println("Botão apertado!");
    bip();
    while (digitalRead(PINO_BOTAO) == LOW) delay(10);  // espera soltar
  }
}

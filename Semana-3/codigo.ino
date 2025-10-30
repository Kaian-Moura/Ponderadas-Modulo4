// Pinos de saída
const int PIN_RED    = 25;
const int PIN_YELLOW = 26;
const int PIN_GREEN  = 27;

// Estado do semáforo com char + ponteiro
char cor;
char* pCor = &cor;

void setLightsFromChar(char* p) {
  char c = *p;                 // derefere o ponteiro
  digitalWrite(PIN_RED,    c == 'R' ? HIGH : LOW);
  digitalWrite(PIN_GREEN,  c == 'G' ? HIGH : LOW);
  digitalWrite(PIN_YELLOW, c == 'Y' ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);

  // tudo apagado no início
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_YELLOW, LOW);
  digitalWrite(PIN_GREEN, LOW);
}

void loop() {
  cor = 'R';                   // 6 s no vermelho
  setLightsFromChar(pCor);
  Serial.println(*pCor);
  delay(6000);

  cor = 'G';                   // 4 s no verde
  setLightsFromChar(pCor);
  Serial.println(*pCor);
  delay(4000);

  cor = 'Y';                   // 2 s no amarelo
  setLightsFromChar(pCor);
  Serial.println(*pCor);
  delay(2000);
}

void setup() {
  // Inicializar o led
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // Isso faz o LED Ligar, deixando a tensão alta
  delay(1000);                      // Aguardar um determinado tempo
  digitalWrite(LED_BUILTIN, LOW);   // Isso faz o LED desligar, deixando a tensão baixa
  delay(1000);                      // Aguardar um determinado tempo
}
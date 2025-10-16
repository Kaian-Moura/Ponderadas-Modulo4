void setup() {
  pinMode(1, OUTPUT); // LED vermelho
  pinMode(2, OUTPUT); // LED verde
  pinMode(3, OUTPUT); // LED laranja
}

void loop() {
    
  digitalWrite(1, HIGH); // acende vermelho
  delay(500);
  digitalWrite(1, LOW);

  digitalWrite(2, HIGH); // acende verde
  delay(500);
  digitalWrite(2, LOW);

  digitalWrite(3, HIGH); // acende laranja
  delay(500);
  digitalWrite(3, LOW);
}

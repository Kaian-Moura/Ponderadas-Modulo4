## Projeto Pisca-pisca (blink)

Vídeo do Arduíno junto com o led funcionando:

<a href="https://youtube.com/shorts/Z8--EmK5UCM">Link do vídeo</a>

Screenshot do código:

<a href="https://res.cloudinary.com/dhlzexsce/image/upload/v1760614285/Captura_de_tela_de_2025-10-16_08-30-21_ukhtzq.png">Link do Screenshot</a>

Código usado:

```c
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
```c

Você também pode verificar o código no arquivo led-pisca-pisca.ino

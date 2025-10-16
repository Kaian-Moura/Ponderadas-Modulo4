## Projeto Pisca-pisca (blink)

### Parte 1:

Vídeo do Arduíno com led interno piscando:

<a href="https://youtube.com/shorts/aGF2YBkhlL0?si=P97d8nXFwEj1iOk1
">Link do vídeo</a>

Screenshot do código:

<img src="https://res.cloudinary.com/dhlzexsce/image/upload/v1760639281/Captura_de_tela_de_2025-10-16_15-27-32_wcpvpo.png" alt="Imagem do Screenshot">

Código usado:

```
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // define o LED interno como saída
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // acende o LED
  delay(1000);                     // espera 1 segundo
  digitalWrite(LED_BUILTIN, LOW);  // apaga o LED
  delay(1000);                     // espera 1 segundo
}

}
```

EXTRA:

Vídeo do Arduíno junto com o led OFF BOARD funcionando:

<a href="https://youtube.com/shorts/Z8--EmK5UCM">Link do vídeo</a>

Screenshot do código:

<img src="https://res.cloudinary.com/dhlzexsce/image/upload/v1760614285/Captura_de_tela_de_2025-10-16_08-30-21_ukhtzq.png" alt="Imagem do Screenshot">

Código usado:

```
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
```

Você também pode verificar o código no arquivo led-pisca-pisca.ino

### Parte 2

Nesta parte foi feito uma simulação de led piscando via TinkerCad.

Segue imagem de como ficou:

<img src="https://res.cloudinary.com/dhlzexsce/image/upload/v1760636578/Captura_de_tela_de_2025-10-16_14-41-56_hpyycd.png" alt="Imagem do Tinkercad">

Código usado:

```
void setup() {
  pinMode(1, OUTPUT); // LED vermelho
  pinMode(2, OUTPUT); // LED verde
  pinMode(3, OUTPUT); // LED laranja
  pinMode(4, OUTPUT); // LED AMARELA
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
  
  digitalWrite(4, HIGH); // acende amarelo
  delay(500);
  digitalWrite(4, LOW);
}


```

Link do TinkerCad:

<a href="https://www.tinkercad.com/things/e1FHGO42X5k-simulacao-piscapisca?sharecode=1owHyjzMuGJgdqmAtDKLPvaaQuPH-Yp-ZDNySG1j4aY">Simulação TinkerCad</a>

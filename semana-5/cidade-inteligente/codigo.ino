#include <WiFi.h>
#include <WebServer.h>

// --------- WiFi ---------
const char* ssid     = "NOME DO WIFI;
const char* password = "senha";

WebServer server(80);

// --------- Pinos dos Semáforos ---------
// Semáforo 1 (via principal)
const int ledVerde1    = 14;
const int ledAmarelo1  = 12;
const int ledVermelho1 = 13;

// Semáforo 2 (via secundária)
const int ledVerde2    = 27;
const int ledAmarelo2  = 26;
const int ledVermelho2 = 25;

// --------- LDR ---------
const int ldrPin = 34;  // entrada analógica do ESP32

// --------- Ultrassônico (HC-SR04) ---------
const int trigPin = 18;
const int echoPin = 19;

// --------- Configurações de Lógica ---------
int   limiarModoNoturnoPercent = 30;   // abaixo disso = noite
float distanciaCarroCm         = 15.0; // abaixo disso = carro detectado na via 2

// Modo forçado via interface
bool modoForcadoDia   = false;
bool modoForcadoNoite = false;

// Valores globais para mostrar na página
int   ldrPercentGlobal   = 0;
float distanciaGlobalCm  = -1.0;

// Para detectar "chegada" do carro (borda de subida)
bool carroEstavaPerto = false;

// ===================== Funções auxiliares =====================

void desligaTodos() {
  digitalWrite(ledVerde1, LOW);
  digitalWrite(ledAmarelo1, LOW);
  digitalWrite(ledVermelho1, LOW);

  digitalWrite(ledVerde2, LOW);
  digitalWrite(ledAmarelo2, LOW);
  digitalWrite(ledVermelho2, LOW);
}

int lerLdrPercent() {
  int valorBruto = analogRead(ldrPin);         // 0 ~ 4095 no ESP32
  int porcentagem = map(valorBruto, 0, 4095, 0, 100);
  if (porcentagem < 0)   porcentagem = 0;
  if (porcentagem > 100) porcentagem = 100;
  return porcentagem;
}

// Mede a distância em cm com o HC-SR04
float lerDistanciaCm() {
  // Garante TRIG em LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Pulso de 10us para disparar o sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede tempo de ida e volta (máx ~30ms)
  long duracao = pulseIn(echoPin, HIGH, 30000);

  if (duracao == 0) {
    // Sem leitura válida
    return -1.0;
  }

  // Distância em cm: (duracao / 2) * 0.0343
  float distancia = (duracao / 2.0) * 0.0343;
  return distancia;
}

// Estado padrão de dia: via principal aberta
void estadoPadraoDia() {
  digitalWrite(ledVerde1, HIGH);
  digitalWrite(ledAmarelo1, LOW);
  digitalWrite(ledVermelho1, LOW);

  digitalWrite(ledVerde2, LOW);
  digitalWrite(ledAmarelo2, LOW);
  digitalWrite(ledVermelho2, HIGH);
}

// Atende carro na via 2 (semáforo 2)
void atenderCarroVia2() {
  Serial.println(">>> CARRO DETECTADO NA VIA 2 - iniciando sequencia");

  // Semáforo 1: VERDE -> AMARELO
  digitalWrite(ledVerde1, LOW);
  digitalWrite(ledAmarelo1, HIGH);
  digitalWrite(ledVermelho1, LOW);

  digitalWrite(ledVerde2, LOW);
  digitalWrite(ledAmarelo2, LOW);
  digitalWrite(ledVermelho2, HIGH);
  delay(2000);

  // Ambos VERMELHO (segurando cruzamento)
  digitalWrite(ledVerde1, LOW);
  digitalWrite(ledAmarelo1, LOW);
  digitalWrite(ledVermelho1, HIGH);

  digitalWrite(ledVerde2, LOW);
  digitalWrite(ledAmarelo2, LOW);
  digitalWrite(ledVermelho2, HIGH);
  delay(1000);

  // Semáforo 2: VERDE (carro passando na via secundária)
  digitalWrite(ledVerde2, HIGH);
  digitalWrite(ledAmarelo2, LOW);
  digitalWrite(ledVermelho2, LOW);

  digitalWrite(ledVerde1, LOW);
  digitalWrite(ledAmarelo1, LOW);
  digitalWrite(ledVermelho1, HIGH);
  delay(4000);

  // Semáforo 2: AMARELO
  digitalWrite(ledVerde2, LOW);
  digitalWrite(ledAmarelo2, HIGH);
  digitalWrite(ledVermelho2, LOW);
  delay(2000);

  // Semáforo 2 volta para VERMELHO
  digitalWrite(ledVerde2, LOW);
  digitalWrite(ledAmarelo2, LOW);
  digitalWrite(ledVermelho2, HIGH);
  delay(1000);

  // Volta para estado padrão (via principal aberta)
  estadoPadraoDia();
}

// Modo noturno: pisca amarelo nos dois
void modoNoturnoPisca() {
  Serial.println("MODO NOTURNO - piscando amarelo");

  // Liga amarelos
  digitalWrite(ledVerde1, LOW);
  digitalWrite(ledVermelho1, LOW);
  digitalWrite(ledAmarelo1, HIGH);

  digitalWrite(ledVerde2, LOW);
  digitalWrite(ledVermelho2, LOW);
  digitalWrite(ledAmarelo2, HIGH);
  delay(500);

  // Desliga amarelos
  digitalWrite(ledAmarelo1, LOW);
  digitalWrite(ledAmarelo2, LOW);
  delay(500);
}

String modoTextoAtual(bool noite) {
  if (modoForcadoDia)   return "Dia (forcado pela interface)";
  if (modoForcadoNoite) return "Noite (forcado pela interface)";
  if (noite)            return "Noite (automatico pelo LDR)";
  return "Dia (automatico pelo LDR)";
}

// ===================== Web server =====================

void handleRoot() {
  // Usa os ultimos valores lidos (globais)
  bool noiteAuto = (ldrPercentGlobal < limiarModoNoturnoPercent);
  bool noite;

  if (modoForcadoDia)        noite = false;
  else if (modoForcadoNoite) noite = true;
  else                       noite = noiteAuto;

  String page = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  page += "<title>Semaforo Inteligente</title>";
  page += "<style>";
  page += "body{font-family:Arial;max-width:480px;margin:auto;padding:16px;background:#111;color:#eee;}";
  page += "h1{font-size:22px;margin-bottom:8px;}";
  page += "button{padding:10px 16px;margin:4px 0;width:100%;border:none;border-radius:6px;font-size:16px;}";
  page += ".auto{background:#444;color:#fff;}";
  page += ".dia{background:#2e7d32;color:#fff;}";
  page += ".noite{background:#fbc02d;color:#000;}";
  page += ".card{background:#222;padding:12px;border-radius:8px;margin-bottom:10px;}";
  page += "</style></head><body>";

  page += "<h1>Semaforo Inteligente</h1>";

  page += "<div class='card'>";
  page += "<p><b>LDR:</b> " + String(ldrPercentGlobal) + "%</p>";
  page += "<p><b>Distancia:</b> ";
  if (distanciaGlobalCm < 0) page += "sem leitura";
  else page += String(distanciaGlobalCm, 1) + " cm";
  page += "</p>";
  page += "<p><b>Modo atual:</b> " + modoTextoAtual(noite) + "</p>";
  page += "</div>";

  page += "<div class='card'>";
  page += "<form action='/set' method='GET'>";
  page += "<button class='auto' name='modo' value='auto'>Modo AUTO (LDR decide)</button><br>";
  page += "<button class='dia' name='modo' value='dia'>Forcar MODO DIA</button><br>";
  page += "<button class='noite' name='modo' value='noite'>Forcar MODO NOTURNO</button>";
  page += "</form>";
  page += "</div>";

  page += "<p style='font-size:12px;opacity:0.7;'>Atualize a pagina para ver novos valores.</p>";

  page += "</body></html>";

  server.send(200, "text/html", page);
}

void handleSet() {
  if (server.hasArg("modo")) {
    String m = server.arg("modo");
    if (m == "dia") {
      modoForcadoDia   = true;
      modoForcadoNoite = false;
    } else if (m == "noite") {
      modoForcadoDia   = false;
      modoForcadoNoite = true;
    } else { // auto
      modoForcadoDia   = false;
      modoForcadoNoite = false;
    }
  }
  // redireciona de volta para a raiz
  server.sendHeader("Location", "/");
  server.send(303);
}

// ===================== Setup e Loop =====================

void setup() {
  Serial.begin(115200);

  // Semáforos
  pinMode(ledVerde1, OUTPUT);
  pinMode(ledAmarelo1, OUTPUT);
  pinMode(ledVermelho1, OUTPUT);

  pinMode(ledVerde2, OUTPUT);
  pinMode(ledAmarelo2, OUTPUT);
  pinMode(ledVermelho2, OUTPUT);

  // LDR
  pinMode(ldrPin, INPUT);

  // Ultrassom
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  desligaTodos();
  estadoPadraoDia();

  // Conecta no WiFi
  Serial.print("Conectando em ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Rotas do servidor
  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  server.handleClient(); // trata requisições da interface

  // Lê sensores
  ldrPercentGlobal  = lerLdrPercent();
  distanciaGlobalCm = lerDistanciaCm();

  Serial.print("LDR: ");
  Serial.print(ldrPercentGlobal);
  Serial.print("%  | Distancia: ");
  if (distanciaGlobalCm < 0) Serial.println("sem leitura");
  else {
    Serial.print(distanciaGlobalCm);
    Serial.println(" cm");
  }

  // Decide se é noite ou dia (AUTO)
  bool noiteAuto = (ldrPercentGlobal < limiarModoNoturnoPercent);

  bool noite;
  if (modoForcadoDia)        noite = false;
  else if (modoForcadoNoite) noite = true;
  else                       noite = noiteAuto;

  if (noite) {
    // --------- MODO NOTURNO ---------
    modoNoturnoPisca();
  } else {
    // --------- MODO DIA ---------
    // Estado padrão: via principal aberta (S1 verde, S2 vermelho)
    estadoPadraoDia();

    // Verifica se tem "carro perto" na via 2
    bool carroPerto = (distanciaGlobalCm > 0 && distanciaGlobalCm <= distanciaCarroCm);

    // Dispara a sequência SÓ quando o carro acabou de chegar
    if (carroPerto && !carroEstavaPerto) {
      atenderCarroVia2();
    }

    // Atualiza o estado anterior
    carroEstavaPerto = carroPerto;

    delay(200); // folguinha
  }
}

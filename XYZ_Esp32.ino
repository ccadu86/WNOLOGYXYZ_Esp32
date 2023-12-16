#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "esp";                    // SSID da sua rede Wi-Fi
const char* password = "teste32esp";               // Senha da sua rede Wi-Fi

//////////////////////////////////////////////////////////////////////////////////
const char* broker = "broker.app.wnology.io";          // Endereço do broker MQTT
const int port = 1883;                                 // Porta do broker MQTT
const char* accessKey = "c1723c40-07cf-4ea1-a2a6-304f2d09f2d4";     // Chave de acesso ao broker MQTT
const char* accessSecret = "b2ca687ae0702cfc20c664e7f58b16de164d558f061c5e7a51b6d03cb9ee62fb";  // Segredo de acesso ao broker MQTT
const char* topic = "wnology/657dcf4fa01a6a688e8127e8/state";         // Tópico MQTT a ser assinado
const char* clientID = "657dcf4fa01a6a688e8127e8";     // MQTT Client ID

WiFiClient wifiClient;              // Cliente WiFi
PubSubClient mqttClient(wifiClient); // Cliente MQTT

int X = 0;
int Y = 0;
int Z = 0;
int VACUO = 0;

int AtuadorX = 19;  // define a porta digital como saida
int AtuadorY = 21;  // define a porta digital como saida
int AtuadorZ = 04;  // define a porta digital como saida
int AtuadorV = 18;  // define a porta digital como saida

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial
  setupWiFi();        // Configura a conexão Wi-Fi
  mqttClient.setServer(broker, port); // Configura o servidor MQTT
  mqttClient.setCallback(callback);   // Configura a função de retorno de chamada MQTT

  pinMode(AtuadorX, OUTPUT);  // configura a porta digital como saida
  pinMode(AtuadorY, OUTPUT);  // configura a porta digital como saida
  pinMode(AtuadorZ, OUTPUT);  // configura a porta digital como saida
  pinMode(AtuadorV, OUTPUT);  // configura a porta digital como saida

  digitalWrite(AtuadorX, HIGH);  // configura a porta digital como saida
  digitalWrite(AtuadorY, HIGH);  // configura a porta digital como saida
  digitalWrite(AtuadorZ, HIGH);  // configura a porta digital como saida
  digitalWrite(AtuadorV, HIGH);  // configura a porta digital como saida
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect(); // Reconecta-se ao broker MQTT, se necessário
  }
  mqttClient.loop(); // Mantém a conexão MQTT ativa
  if (X == 1){
      digitalWrite(AtuadorX, LOW);
  }
  else{
      digitalWrite(AtuadorX, HIGH);
  }
  if (Y == 1){
      digitalWrite(AtuadorY, LOW);
  }
  else{
      digitalWrite(AtuadorY, HIGH);
  }
  if (Z == 1){
      digitalWrite(AtuadorZ, LOW);
  }
  else{
      digitalWrite(AtuadorZ, HIGH);
  }
  if (VACUO == 1){
      digitalWrite(AtuadorV, LOW);
  }
  else{
      digitalWrite(AtuadorV, HIGH);
  }
}

void setupWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se à rede Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Conecta-se à rede Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado à rede Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP()); // Imprime o endereço IP atribuído
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao broker MQTT...");

    if (mqttClient.connect(clientID, accessKey, accessSecret)) {
      Serial.println("Conectado");
      mqttClient.subscribe(topic); // Assina o tópico MQTT
    } else {
      Serial.print("Falha ao conectar, código de erro: ");
      Serial.print(mqttClient.state()); // Imprime o código de erro de conexão MQTT
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida do tópico: ");
  Serial.println(topic); // Imprime o tópico da mensagem recebida

  DynamicJsonDocument doc(256);  // Cria um documento JSON dinâmico com tamanho de 256 bytes
  deserializeJson(doc, payload, length);  // Analisa o payload JSON recebido

  const JsonObject& data = doc["data"];  // Obtém o objeto JSON "data" do documento

  if (data.containsKey("X")) {
    X = data["X"].as<int>();  // Extrai o valor de "TEMP" e atribui à variável temperatura
    Serial.print("X: ");
    Serial.println(X); // Imprime a temperatura    
  }
  if (data.containsKey("Y")) {
    Y = data["Y"].as<int>();  // Extrai o valor de "TEMP" e atribui à variável temperatura
    Serial.print("Y: ");
    Serial.println(Y); // Imprime a temperatura
  }
  if (data.containsKey("Z")) {
    Z = data["Z"].as<int>();  // Extrai o valor de "TEMP" e atribui à variável temperatura
    Serial.print("Z: ");
    Serial.println(Z); // Imprime a temperatura
  }
  if (data.containsKey("VACUO")) {
    VACUO = data["VACUO"].as<int>();  // Extrai o valor de "TEMP" e atribui à variável temperatura
    Serial.print("VACUO: ");
    Serial.println(VACUO); // Imprime a temperatura
  }

}

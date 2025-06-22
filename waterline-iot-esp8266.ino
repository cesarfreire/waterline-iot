#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "secrets.h"
#include "DHT.h"

// sensor nivel agua
#define sensorVcc D3
#define sensorSinal A0

// pino alimentar agora
#define PINO_ALIMENTADOR D0

// reles
#define PINO_RELE_1 D5
#define PINO_RELE_2 D6
#define PINO_RELE_3 D7
#define PINO_RELE_4 D8

// sensor temperatura agua
#define PINO_ONEWIRE D4 // Define pino do sensor
OneWire oneWire(PINO_ONEWIRE); // Cria um objeto OneWire
DallasTemperature sensorAgua(&oneWire); // Informa a referencia da biblioteca dallas temperature para Biblioteca onewire
DeviceAddress endereco_temp; // Cria um endereco temporario da leitura do sensor
 
// sensor temperatura e umidade
#define DHTPIN 4        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

float h; // variavel umidade
float t; // variavel temperatura
float wt; // variavel temperatura agua
int wl; // variavel nivel agua

// Valores anteriores
float lastH = -1;
float lastT = -1;
float lastWT = -1;
int lastWL = -1;

int val = 0; // variavel para leitura nivel agua
unsigned long lastMillis = 0;
const long interval = 60000; // 1min

// variável para o led builtin
bool ledAceso = false;
 
// topicos MQTT
#define AWS_IOT_PUBLISH_TOPIC   "esp8266/sensors"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/commands"
#define AWS_IOT_LOG_TOPIC "esp8266/logs"

// wifi
WiFiClientSecure net;

// certificado iot
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
PubSubClient client(net);
 
time_t now;
time_t nowish = 1510592825;
 
 
void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");

  // para utc-3
  // configTime(TIME_ZONE * 3600, 0, "a.st1.ntp.br", "a.ntp.br");

  // para utc
  configTime("GMT0", "a.st1.ntp.br", "a.ntp.br");
  
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");

  struct tm timeinfo;
  // para utc-3
  // localtime_r(&now, &timeinfo);
  
  // para utc
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time (UTC): ");
  Serial.print(asctime(&timeinfo));
}
 
 
void messageReceived(char *topic, byte *payload, unsigned int length)
{
  // Criando um buffer para armazenar o JSON recebido
  StaticJsonDocument<256> doc;

  // Deserializando o payload recebido
  DeserializationError error = deserializeJson(doc, payload, length);
  if (error)
  {
    Serial.print(F("Failed to deserialize JSON: "));
    Serial.println(error.f_str());
    return;
  }

  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  // Exibindo o JSON completo no Serial
  serializeJson(doc, Serial);
  Serial.println();

  if (doc.containsKey("action"))
  {
    String action = doc["action"].as<String>();

    // Prepara a mensagem de log
    StaticJsonDocument<256> logDoc;
    char timeStr[25];
    time_t logTime = time(nullptr);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%SZ", gmtime(&logTime));
    logDoc["timestamp"] = timeStr;
    logDoc["action"] = action;

    if (action == "LIGA_RELE_1")
    {
      digitalWrite(PINO_RELE_1, LOW); // liga rele 1
      Serial.println("RELE 1 ligado.");
      logDoc["status"] = "RELE 1 ligado";
      // Publica o log no tópico de log
      char logBuffer[512];
      serializeJson(logDoc, logBuffer);
      client.publish(AWS_IOT_LOG_TOPIC, logBuffer);  // Envia o log
    }
    else if (action == "DESLIGA_RELE_1")
    {
      digitalWrite(PINO_RELE_1, HIGH); // desliga rele 1
      Serial.println("RELE 1 desligado.");
      logDoc["status"] = "RELE 1 desligado";
      // Publica o log no tópico de log
      char logBuffer[512];
      serializeJson(logDoc, logBuffer);
      client.publish(AWS_IOT_LOG_TOPIC, logBuffer);  // Envia o log
    }
    else if (action == "LIGA_RELE_2")
    {
      digitalWrite(PINO_RELE_2, LOW); // liga rele 2
      Serial.println("RELE 2 ligado.");
      logDoc["status"] = "RELE 2 ligado";
      // Publica o log no tópico de log
      char logBuffer[512];
      serializeJson(logDoc, logBuffer);
      client.publish(AWS_IOT_LOG_TOPIC, logBuffer);  // Envia o log
    }
    else if (action == "DESLIGA_RELE_2")
    {
      digitalWrite(PINO_RELE_2, HIGH); // desliga rele 2
      Serial.println("RELE 2 desligado.");
      logDoc["status"] = "RELE 2 desligado";
      // Publica o log no tópico de log
      char logBuffer[512];
      serializeJson(logDoc, logBuffer);
      client.publish(AWS_IOT_LOG_TOPIC, logBuffer);  // Envia o log
    }
    else if (action == "LIGA_RELE_3")
    {
      digitalWrite(PINO_RELE_3, LOW); // liga rele 3
      Serial.println("RELE 3 ligado.");
      logDoc["status"] = "RELE 3 ligado";
      // Publica o log no tópico de log
      char logBuffer[512];
      serializeJson(logDoc, logBuffer);
      client.publish(AWS_IOT_LOG_TOPIC, logBuffer);  // Envia o log
    }
    else if (action == "DESLIGA_RELE_3")
    {
      digitalWrite(PINO_RELE_3, HIGH); // desliga rele 3
      Serial.println("RELE 3 desligado.");
      logDoc["status"] = "RELE 3 desligado";
      // Publica o log no tópico de log
      char logBuffer[512];
      serializeJson(logDoc, logBuffer);
      client.publish(AWS_IOT_LOG_TOPIC, logBuffer);  // Envia o log
    }
    else if (action == "LIGA_RELE_4")
    {
      digitalWrite(PINO_RELE_4, LOW); // liga rele 4
      Serial.println("RELE 4 ligado.");
      logDoc["status"] = "RELE 4 ligado";
      // Publica o log no tópico de log
      char logBuffer[512];
      serializeJson(logDoc, logBuffer);
      client.publish(AWS_IOT_LOG_TOPIC, logBuffer);  // Envia o log
    }
    else if (action == "DESLIGA_RELE_4")
    {
      digitalWrite(PINO_RELE_4, HIGH); // desliga rele 4
      Serial.println("RELE 4 desligado.");
      logDoc["status"] = "RELE 4 desligado";
      // Publica o log no tópico de log
      char logBuffer[512];
      serializeJson(logDoc, logBuffer);
      client.publish(AWS_IOT_LOG_TOPIC, logBuffer);  // Envia o log
    }
    else if (action == "ALIMENTAR_AGORA")
    {
      Serial.println("Iniciando alimentação...");
      digitalWrite(PINO_ALIMENTADOR, LOW); // Ativa o pino (nível baixo)
      delay(500); // Mantém ativado por 500 ms (meio segundo)
      digitalWrite(PINO_ALIMENTADOR, HIGH); // Desativa o pino (nível alto)
      Serial.println("Alimentador acionado.");
      logDoc["status"] = "Alimentação efetuada";
      // Publica o log no tópico de log
      char logBuffer[512];
      serializeJson(logDoc, logBuffer);
      client.publish(AWS_IOT_LOG_TOPIC, logBuffer);  // Envia o log
    }
    else
    {
      Serial.println("Ação não reconhecida.");
    }
  }
  else
  {
    Serial.println("A chave 'ACTION' não foi encontrada no JSON.");
  }
}
 
 
void connectAWS()
{
  delay(3000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
 
  NTPConnect();
 
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
 
  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);
 
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }
 
  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
void publishMessage()
{
  StaticJsonDocument<256> doc;
  // doc["time"] = millis();
  char timeStr[25];
  time_t now = time(nullptr);
  // Formata a data/hora como "YYYY-MM-DDTHH:MM:SSZ"
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));

  // Adiciona o timestamp ao JSON em vez de millis()
  doc["timestamp"] = timeStr;
  doc["humidity"] = h;
  doc["temperature"] = t;
  doc["water_temperature"] = wt;
  doc["water_level"] = wl;
  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

/* leitura do sensor */
int readWaterLevel() {  
  val = analogRead(sensorSinal);
  return val;
}
 
 
void setup()
{
  Serial.begin(9600);
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));

  // define modo dos pinos dos reles como output
  pinMode(PINO_RELE_1, OUTPUT);
  pinMode(PINO_RELE_2, OUTPUT);
  pinMode(PINO_RELE_3, OUTPUT);
  pinMode(PINO_RELE_4, OUTPUT);
  pinMode(PINO_ALIMENTADOR, OUTPUT);

  // reles iniciam como desligados
  digitalWrite(PINO_RELE_1, HIGH);
  digitalWrite(PINO_RELE_2, HIGH);
  digitalWrite(PINO_RELE_3, HIGH);
  digitalWrite(PINO_RELE_4, HIGH);
  digitalWrite(PINO_ALIMENTADOR, HIGH);
  

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(sensorVcc, OUTPUT);
  digitalWrite(sensorVcc, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  
  
  connectAWS();
  sensorAgua.begin();
  dht.begin();
}
 
 
void loop()
{
  now = time(nullptr);

  if (!client.connected()) {
    connectAWS();
  } else {
    client.loop();
  }

  digitalWrite(LED_BUILTIN, ledAceso ? LOW : HIGH);

  unsigned long currentMillis = millis();

  // Verifica se é hora de atualizar os sensores
  if (currentMillis - lastMillis >= interval) {
    lastMillis = currentMillis;

    h = dht.readHumidity();
    t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    sensorAgua.requestTemperatures();
    if (!sensorAgua.getAddress(endereco_temp, 0)) {
      Serial.println(F("Failed to read from water sensor!"));
      return;
    } else {
      wt = sensorAgua.getTempC(endereco_temp);
    }

    wl = readWaterLevel();
    if (isnan(wl)) {
      Serial.println(F("Failed to read water level!"));
      return;
    }

    Serial.printf("Umidade: %.2f %% | Temp: %.2f °C | Água: %.2f °C | Nível: %d\n", h, t, wt, wl);

    // Envia apenas se houver alterações
    if (h != lastH || t != lastT || wt != lastWT || wl != lastWL) {
      publishMessage();
      lastH = h;
      lastT = t;
      lastWT = wt;
      lastWL = wl;
    }
  }
}
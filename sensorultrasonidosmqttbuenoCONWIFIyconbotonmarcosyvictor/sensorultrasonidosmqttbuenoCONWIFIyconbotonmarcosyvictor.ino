#include <WiFi.h>
#include <PubSubClient.h>
#include "time.h"

// Pines del sensor ultrasónico
const int trigPin = 18;
const int echoPin = 19;

// Pines de los LEDs
const int redLED = 2;
const int yellowLED = 4;
const int greenLED = 23;

// WiFi
const char* ssid = "ALUMNOS_CICLOS";
const char* password = "Ciclos2025sz?";

// NTP
const char* ntpServer = "es.pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 0;

// MQTT
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_user = "marcos";
const char* mqtt_pass = "1234";
const char* mqtt_client_id = "mqttdash-6cf69e18";
const char* mqtt_topic_estado = "sensor/estado";
const char* mqtt_topic_control = "sensor/control";

WiFiClient espClient;
PubSubClient mqtt_client(espClient);
struct tm timeinfo;

// Variable de control
bool habilitado = false;

// Función callback para recibir comandos MQTT (usando "1" y "0")
void callback(char* topic, byte* payload, unsigned int length) {
  String mensaje = "";
  for (unsigned int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensaje);

  if (String(topic) == mqtt_topic_control) {
    if (mensaje == "1") {
      habilitado = true;
      Serial.println("Ejecución habilitada.");
    } else if (mensaje == "0") {
      habilitado = false;
      Serial.println("Ejecución detenida.");
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(callback);
}

void reconnect() {
  while (!mqtt_client.connected()) {
    Serial.print("Conectando al broker MQTT...");
    String client_id = "esp32-client-" + String(WiFi.macAddress());
    if (mqtt_client.connect(client_id.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Conectado.");
      mqtt_client.subscribe(mqtt_topic_control);  // Suscripción al tema de control
    } else {
      Serial.print("Fallo. Estado: ");
      Serial.println(mqtt_client.state());
      delay(5000);
    }
  }
}

float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duracion = pulseIn(echoPin, HIGH);
  return duracion * 0.0343 / 2;  // cm
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }

  if (!mqtt_client.connected()) {
    reconnect();
  }

  mqtt_client.loop();

  if (!habilitado) {
    // Apaga los LEDs si está deshabilitado
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    delay(500);
    return;
  }

  float distancia = medirDistancia();
  String estadoLED;

  if (distancia > 0 && distancia <= 30) {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    estadoLED = "ROJO";
  } else if (distancia > 30 && distancia <= 45) {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
    estadoLED = "AMARILLO";
  } else if (distancia > 45 && distancia <= 100) {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
    estadoLED = "VERDE";
  } else {
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    estadoLED = "NINGUNO";
  }

  if (getLocalTime(&timeinfo)) {
    char horaStr[30];
    strftime(horaStr, sizeof(horaStr), "%d/%m/%Y %H:%M:%S", &timeinfo);

    String mensaje = "Hora: " + String(horaStr) +
                     " | LED: " + estadoLED +
                     " | Distancia: " + String(distancia, 2) + " cm";

    mqtt_client.publish(mqtt_topic_estado, mensaje.c_str());
    Serial.println(mensaje);
  } else {
    Serial.println("Error al obtener la hora.");
  }

  delay(1000);  // 1 segundo entre lecturas
}
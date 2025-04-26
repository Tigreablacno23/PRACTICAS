
#include <WiFi.h> // Librería para manejar conexión WiFi
#include "time.h" // Librería para obtener fecha y hora desde Internet (NTP)
#include <stdint.h> // Librería estándar para tipos de datos como int32_t
#include <PubSubClient.h> // Librería para comunicación MQTT

// Configuración de WiFi
const char* ssid     = "MiFibra-E7B0";  // Nombre de la red WiFi
const char* pasword  = "RwwXq3xu";      // Contraseña de la red WiFi

// Configuración del servidor NTP (para fecha y hora)
const char* ntpServer = "es.pool.ntp.org"; // Servidor de hora
const int32_t gmtOffset_sec = 3600;        // Offset de zona horaria (+1 hora)
const int32_t daylightOffset_sec = 0;      // No hay horario de verano

// Estructura para almacenar la fecha y hora
struct tm timeinfo ;

// Configuración del servidor MQTT
const char* mqtt_username = "marcos";  // Usuario MQTT (no usado en este código)
const char* mqtt_password = "1234";    // Contraseña MQTT (no usado en este código)
const char* server = "broker.emqx.io"; // Dirección del broker MQTT
const int mqtt_port = 1883;            // Puerto del broker MQTT

WiFiClient espClient;         // Crea un cliente WiFi
PubSubClient mqtt_client(espClient); // Crea un cliente MQTT usando el WiFi

void setup() {
  Serial.begin(115200); // Inicia la comunicación Serial a 115200 baudios

  // Conexión a la red WiFi
  Serial.println("Conectando a Wifi...");
  WiFi.begin(ssid, pasword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Espera 1 segundo
    Serial.print("."); // Imprime puntos mientras se conecta
  }
  Serial.println("\nConexion Wifi establecida."); // Ya conectado
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP()); // Imprime la IP local asignada al ESP32

  // Configura la hora local usando el servidor NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Configura el servidor MQTT
  mqtt_client.setServer(server, mqtt_port);
}

void loop() {
  mqtt_client.loop(); // Mantiene la conexión MQTT activa y procesa mensajes entrantes

  getLocalTime(&timeinfo); // Actualiza la estructura timeinfo con la fecha y hora actual

  Serial.println(&timeinfo, "%d %m %Y %H:%M:%S"); // Imprime la fecha y hora en formato día mes año hora:minutos:segundos

  // Crea una cadena con la fecha actual en formato día/mes/año
  String currentTime = String(timeinfo.tm_mday) + "/" + String(timeinfo.tm_mon + 1) + "/" + String(timeinfo.tm_year + 1900);

  // Publica un mensaje en el topic "sensor/hora" si está conectado al broker MQTT
  if (mqtt_client.connected()) {
    mqtt_client.publish("sensor/hora", "Mensaje"); // Ahora mismo envía "Mensaje" fijo (no envía currentTime)
  }

  delay(1000); // Espera 1 segundo antes de repetir el bucle
}
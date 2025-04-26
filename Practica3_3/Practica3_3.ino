#include <WiFi.h>           // Librería para conexión WiFi
#include "time.h"           // Librería para obtener hora desde Internet
#include <stdint.h>         // Tipos de datos estándar
#include <PubSubClient.h>   // Librería para cliente MQTT

// Configuración de WiFi
const char* ssid     = "MiFibra-E7B0";   // Nombre de la red WiFi
const char* pasword  = "RwwXq3xu";       // Contraseña WiFi

// Configuración de servidor NTP (hora)
const char* ntpServer = "es.pool.ntp.org"; // Servidor NTP español
const int32_t gmtOffset_sec = 3600;         // Offset horario (GMT+1)
const int32_t daylightOffset_sec = 0;       // Sin horario de verano

// Estructura para almacenar fecha y hora
struct tm timeinfo;

// Configuración de servidor MQTT
const char* mqtt_username = "marcos";       // Usuario MQTT
const char* mqtt_password = "1234";          // Contraseña MQTT
const char* server = "broker.emqx.io";       // Broker público
const int mqtt_port = 1883;                  // Puerto MQTT

WiFiClient espClient;             // Cliente WiFi
PubSubClient mqtt_client(espClient); // Cliente MQTT usando WiFi

// Callback para recibir mensajes MQTT
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Mensaje recibido en el tópico: ");
    Serial.println(topic);

    // Convertir el mensaje de bytes a String
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Mensaje: ");
    Serial.println(message);

    // Controlar el LED con el mensaje recibido
    if (message == "1") {
        Serial.println("ON");
        digitalWrite(LED_BUILTIN, HIGH); // Encender LED
    }
    else if (message == "0") {
        Serial.println("OFF");
        digitalWrite(LED_BUILTIN, LOW);  // Apagar LED
    }
}

// Función para reconectar a MQTT si se pierde la conexión
void reconnect() {
    while (!mqtt_client.connected()) {
        Serial.print("Conectando a MQTT...");
        
        // Crear un client_id único basado en la MAC
        String client_id = "esp32-client-" + String(WiFi.macAddress());

        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Conectado");

            // Suscribirse al tópico para recibir órdenes
            mqtt_client.subscribe("sensor/boton");
        } else {
            Serial.print("Error, código: ");
            Serial.println(mqtt_client.state());
            delay(5000); // Esperar antes de intentar otra vez
        }
    }
}

void setup() {
    Serial.begin(115200);         // Iniciar comunicación Serial
    pinMode(LED_BUILTIN, OUTPUT); // Configurar LED interno como salida

    // Conexión a WiFi
    Serial.println("Conectando a Wifi...");
    WiFi.begin(ssid, pasword);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConexion Wifi establecida.");
    Serial.print("Direccion IP: ");
    Serial.println(WiFi.localIP());

    // Configurar la hora desde el servidor NTP
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // Configurar el servidor MQTT
    mqtt_client.setServer(server, mqtt_port);
    mqtt_client.setCallback(callback); // Establecer la función para recibir mensajes
}

void loop() {
    // Si no está conectado a MQTT, reconectar
    if (!mqtt_client.connected()) {
        reconnect();
    }
    
    mqtt_client.loop(); // Procesar mensajes entrantes/salientes de MQTT

    // Obtener la fecha y hora actuales
    getLocalTime(&timeinfo);
    Serial.println(&timeinfo, "%d %m %Y %H:%M:%S");

    // Crear un String con la fecha en formato "dia/mes/año"
    String currentTime = String(timeinfo.tm_mday) + "/" + String(timeinfo.tm_mon + 1) + "/" + String(timeinfo.tm_year + 1900);

    // Publicar la fecha actual (aunque ahora mismo publicas texto fijo "Mensaje")
    if (mqtt_client.connected()) {
        mqtt_client.publish("sensor/hora", "Mensaje"); // Deberías cambiar "Mensaje" por currentTime.c_str() si quieres publicar la fecha
    }

    delay(1000); // Esperar 1 segundo antes de repetir
}


#include <stdint.h>
#include <WiFi.h>    // Incluye librería para conectarse al WiFi
#include "time.h"    // Incluye librería para manejar fecha y hora (NTP)

//  Datos de WiFi 
const char* ssid = "MiFibra-E7B0";      // Nombre de la red WiFi
const char* password = "RwwXq3xu";      // Contraseña del WiFi

//  Datos del servidor NTP 
const char* ntpServer = "pool.ntp.org"; // Servidor público de hora
const long gmtOffset_sec = 3600;        // Diferencia horaria con UTC (+1 hora = 3600 segundos)
const int daylightOffset_sec = 0;       // Horario de verano (0 por ahora)

//  Estructura para almacenar fecha y hora 
struct tm timeinfo;  // timeinfo guardará la información de fecha/hora

void setup() {
  Serial.begin(115200);  // Inicia comunicación serial para imprimir mensajes

  //  Conexión a WiFi 
  Serial.println("Conectando a Wifi...");
  WiFi.begin(ssid, password);    // Inicia conexión WiFi

  // Mientras no estemos conectados, espera
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);       // Espera 0,5 segundos
    Serial.print("."); // Imprime puntos mientras intenta conectar
  }
  
  // Una vez conectado
  Serial.println("\nConectado a Wifi.");
  Serial.println(WiFi.localIP()); // Muestra la IP que ha obtenido

  //  Configuración de la hora (NTP) 
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  delay(2000); // Espera 2 segundos para asegurarse que se conecta al servidor NTP

  Serial.println("Intentando obtener la hora...");

  // Intentar obtener hora inicial
  if (!getLocalTime(&timeinfo)) {   // Si falla obtener la hora...
    Serial.println("Fallo al obtener hora"); // Mensaje de error
    return; // Salir del setup
  }

  // Hora recibida correctamente
  Serial.println("Hora obtenida correctamente.");
  Serial.println(&timeinfo, "%d/%m/%Y %H:%M:%S");  // Muestra la fecha y hora
}

void loop() {
  // Cada segundo se vuelve a consultar la hora actual
  if (getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "%d/%m/%Y %H:%M:%S"); // Imprime la fecha y hora
  }
  
  delay(1000); // Espera 1 segundo entre lecturas
}

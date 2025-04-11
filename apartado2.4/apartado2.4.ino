
#include <stdint.h>
#include <DHT.h>

// Definiciones de pines
#define DHTPIN 23          // Pin donde está conectado el DHT11
#define DHTTYPE DHT11     // Definimos el tipo de sensor DHT

// Inicializamos el sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Definición de pines para los LEDs como uint8_t
uint8_t LED_VERDE = 2;
uint8_t LED_AZUL = 4;
uint8_t LED_ROJO = 18;

void setup() {
  // Inicializamos la comunicación serial
  Serial.begin(9600);
  
  // Inicializamos el sensor DHT
  dht.begin();
  
  // Configuramos los pines de los LEDs como salida
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
}

void loop() {
  // Esperamos un segundo entre lecturas
  delay(2000);
  
  // Leemos la temperatura y la humedad
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Temperatura en Celsius

  // Comprobamos si las lecturas han fallado
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer el sensor DHT!");
    return;
  }

  // Mostramos los valores en el monitor serial
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C");

  // Control de LEDs según las condiciones
  if (t < 28) {
    digitalWrite(LED_VERDE, HIGH); // Activa LED verde
  } else {
    digitalWrite(LED_VERDE, LOW); // Desactiva LED verde
  }

  if (t > 30) {
    digitalWrite(LED_AZUL, HIGH); // Activa LED azul
  } else {
    digitalWrite(LED_AZUL, LOW); // Desactiva LED azul
  }

  if (t < 28 && t > 30 && h < 70) {
    digitalWrite(LED_ROJO, HIGH); // Activa LED rojo
  } else {
    digitalWrite(LED_ROJO, LOW); // Desactiva LED rojo
  }
}
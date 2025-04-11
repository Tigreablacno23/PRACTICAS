
#include <stdint.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Tamaño de la pantalla
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Pin de reset de la pantalla (no se usa)
#define OLED_RESET -1

// Objeto display OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin del sensor de infrarrojos
#define IR_SENSOR_PIN 18

void setup() {
  Serial.begin(115200);

  // Configurar pin del sensor IR como entrada
  pinMode(IR_SENSOR_PIN, INPUT);

  // Inicializar la pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("No se encontró la pantalla OLED"));
    while (true); // Se detiene si no la encuentra
  }

  // Mensaje de inicio
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Sistema IR");
  display.display();
  delay(2000);
}

void loop() {
  int sensorValue = digitalRead(IR_SENSOR_PIN);

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 20);

  if (sensorValue == LOW) {
    display.println("Obstaculo!");
    Serial.println("Obstaculo detectado");
  } else {
    display.println("Libre :)");
    Serial.println("Sin obstaculo");
  }

  display.display();
  delay(500);
}
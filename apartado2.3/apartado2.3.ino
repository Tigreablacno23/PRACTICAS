const int pirPin = 4;     
const int buzzerPin = 23;


void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  int movimiento = digitalRead(pirPin);

  if (movimiento == HIGH) {
    digitalWrite(buzzerPin, HIGH);
    Serial.println("¡Movimiento detectado!");
  } else {
    digitalWrite(buzzerPin, LOW);
    Serial.println("Sin movimiento");
  }

  delay(100); // estabilidad
}
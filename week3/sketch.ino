#include <WiFi.h>

const int TRIG_PIN = 18;
const int ECHO_PIN = 5;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
int notConnectedCounter = 0;

unsigned long previousMillis = 0;
const long telemetryInterval = 5000;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.print("Connecting to Virtual Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    notConnectedCounter++;
    if (notConnectedCounter > 150) {
      ESP.restart();
    }
  }
  Serial.println("\nConnected!");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= telemetryInterval) {
    previousMillis = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG_PIN, LOW);

      long duration = pulseIn(ECHO_PIN, HIGH);
      float distance = (duration / 2) * 0.0343;

      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");

      char payload[10];
      dtostrf(distance, 4, 2, payload);
      Serial.print("Payload: ");
      Serial.println(payload);
    } else {
      WiFi.begin(ssid, password);
    }
  }
}
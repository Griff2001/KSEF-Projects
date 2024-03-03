
#define BLYNK_TEMPLATE_ID "TMPL24u1nS5_d"
#define BLYNK_TEMPLATE_NAME "Agriculture"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


// Blynk authentication token and WiFi credentials
char auth[] = "x5UKdAPqJM-kocN5MrT3FZb8Abyf-uLp";
char ssid[] = "Wegner";
char pass[] = "Wegner123!";

#define PUMP_PIN D1
#define MOISTURE_SENSOR_PIN A0

BlynkTimer timer;

void setup() {
  pinMode(PUMP_PIN, OUTPUT);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, checkMoisture); // Check moisture every second
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkMoisture() {
  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);

  if (moistureValue < 500) { // Adjust threshold as needed
    Serial.println("Soil is moist. Pump is off.");
    digitalWrite(PUMP_PIN, LOW); // Turn off the pump
    Blynk.virtualWrite(V3, LOW); // Write LOW to Virtual Pin V3 to turn off the pump in Blynk app
  } else {
    Serial.println("Moisture detected! Turning on the pump.");
    digitalWrite(PUMP_PIN, HIGH); // Turn on the pump
    Blynk.virtualWrite(V3, HIGH); // Write HIGH to Virtual Pin V3 to turn on the pump in Blynk app
  }
}

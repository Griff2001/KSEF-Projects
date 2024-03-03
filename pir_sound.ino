#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define PIR_PIN D1   // PIR sensor pin
#define LED_PIN D2   // LED pin
#define BUZZER_PIN D3  // Buzzer pin
#define LED_VPIN V2   // Virtual pin for controlling LED in Blynk app

// Blynk authentication token and WiFi credentials
char auth[] = "x5UKdAPqJM-kocN5MrT3FZb8Abyf-uLp";
char ssid[] = "Wegner";
char pass[] = "Wegner123!";

bool motionDetected = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  int pirValue = digitalRead(PIR_PIN);

  if (pirValue == HIGH) {
    if (!motionDetected) {
      handleMotionDetected();
    }
  } else {
    if (motionDetected) {
      handleMotionStopped();
    }
  }

  Blynk.run();
}

void handleMotionDetected() {
  Serial.println("Motion detected!");
  motionDetected = true;
  digitalWrite(LED_PIN, HIGH);  // Turn on LED
  tone(BUZZER_PIN, 1000);  // Turn on buzzer
  Blynk.virtualWrite(LED_VPIN, HIGH); // Turn on LED in Blynk app
}

void handleMotionStopped() {
  Serial.println("Motion stopped.");
  motionDetected = false;
  digitalWrite(LED_PIN, LOW);  // Turn off LED
  noTone(BUZZER_PIN);  // Turn off buzzer
  Blynk.virtualWrite(LED_VPIN, LOW); // Turn off LED in Blynk app
}

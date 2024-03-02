#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define PIR_PIN D1   // PIR sensor pin
#define LED_PIN D2   // LED pin
#define BUZZER_PIN D3  // Buzzer pin

char auth[] = "YourAuthToken";  // Blynk authentication token
char ssid[] = "YourWiFiSSID";   // Your WiFi SSID
char pass[] = "YourWiFiPassword"; // Your WiFi password

bool motionDetected = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  if (motionDetected != checkMotion()) {
    motionDetected = !motionDetected;
    if (motionDetected) {
      turnOnLED();
      turnOnBuzzer();
      Blynk.notify("Motion detected!");
    } else {
      turnOffLED();
      turnOffBuzzer();
    }
  }

  Blynk.run();
}

bool checkMotion() {
  return digitalRead(PIR_PIN) == HIGH;
}

void turnOnLED() {
  digitalWrite(LED_PIN, HIGH);
}

void turnOffLED() {
  digitalWrite(LED_PIN, LOW);
}

void turnOnBuzzer() {
  tone(BUZZER_PIN, 1000);
}

void turnOffBuzzer() {
  noTone(BUZZER_PIN);
}

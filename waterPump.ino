#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

// Blynk authentication token and WiFi credentials
char auth[] = "x5UKdAPqJM-kocN5MrT3FZb8Abyf-uLp";
char ssid[] = "Wegner";
char pass[] = "Wegner123!";

/* Virtual pin for controlling blynk app */
#define PUMP_CONTROL_VPIN V3

/* Pin definitions */
#define PUMP_PIN D4 
#define MOISTURE_PIN A0 

bool automaticMode = true; // Flag to indicate automatic mode

BLYNK_WRITE(PUMP_CONTROL_VPIN){
  digitalWrite(PUMP_PIN, param.asInt());
  automaticMode = false; // Switch to manual mode when Blynk control is activated
}

void setup() {
  pinMode(PUMP_PIN, OUTPUT); 
  pinMode(MOISTURE_PIN, INPUT);
  Serial.begin(9600); 

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();
  if (automaticMode) {
    int moistureLevel = map(analogRead(MOISTURE_PIN), 0, 1023, 0, 100); // Read the moisture level and map it to a range of 0-100
    Serial.print("Moisture Level: ");
    Serial.println(moistureLevel); // Print the moisture level

    if (moistureLevel > 80) { // If moisture level is above 80
      // Turn the pump off
      digitalWrite(PUMP_PIN, LOW);
      Serial.println("Pump OFF");
    } else if (moistureLevel < 70) { // If moisture level is below 70
      // Turn the pump on
      digitalWrite(PUMP_PIN, HIGH);
      Serial.println("Pump ON");
    }
  }
  delay(1000); // Wait for 1 second before checking moisture level again
}

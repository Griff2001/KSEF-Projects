#define PUMP_PIN D4 // Define the pin connected to the transistor
#define MOISTURE_PIN A0 // Define the pin connected to the moisture sensor

void setup() {
  pinMode(PUMP_PIN, OUTPUT); // Set the pump pin as an output
  pinMode(MOISTURE_PIN, INPUT);
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
}

void loop() {
  int moistureLevel = map(analogRead(MOISTURE_PIN), 0, 1023, 1000, 0); // Read the moisture level and map it to a range of 0-1000
  Serial.print("Moisture Level: ");
  Serial.println(moistureLevel); // Print the moisture level

  if (moistureLevel > 80) { // If moisture level is above 80
    // Turn the pump off
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("Pump OFF");
  } else if (moistureLevel < 70) { // If moisture level is below 600
    // Turn the pump on
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("Pump ON");
  }

  delay(1000); // Wait for 1 second before checking moisture level again
}

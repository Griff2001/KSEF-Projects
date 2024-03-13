#include <SoftwareSerial.h>

#define GSM_TX D6  // Connect GSM module TX to NodeMCU D1
#define GSM_RX D5  // Connect GSM module RX to NodeMCU D2

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

void setup() {
  Serial.begin(9600);   // Initialize serial communication for debugging
  gsmSerial.begin(9600); // Initialize GSM serial communication

  Serial.println("Initializing GSM module...");

  delay(2000); // Give time for the GSM module to initialize

  // AT command to test if the GSM module is responding
  gsmSerial.println("AT");
}

void loop() {
  if (gsmSerial.available()) {
    char c = gsmSerial.read();
    Serial.write(c); // Print response from GSM module to serial monitor
  }

  // Make a call
  makeCall();

  // Add a delay before making another call (if needed)
  delay(60000); // Delay for 1 minute (60000 milliseconds)
}

void makeCall() {
  // AT command to make a call
  gsmSerial.print("ATD+254740390420;"); // Replace +123456789 with the phone number you want to call
  delay(1000);
  gsmSerial.println();
}

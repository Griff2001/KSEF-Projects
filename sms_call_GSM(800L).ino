//using arduino

#include <SoftwareSerial.h>

#define GSM_TX 6  // Connect GSM module TX to Arduino Pin 6
#define GSM_RX 7  // Connect GSM module RX to Arduino Pin 7

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

void setup() {
  Serial.begin(9600);   // Initialize serial communication for debugging
  gsmSerial.begin(9600); // Initialize GSM serial communication

  Serial.println("Initializing GSM module...");

  delay(1000); // Give time for the GSM module to initialize

  // AT command to test if the GSM module is responding
  gsmSerial.println("AT");
}

void loop() {
  if (gsmSerial.available()) {
    char c = gsmSerial.read();
    Serial.write(c); // Print response from GSM module to serial monitor
  }

  // Send SMS after 3 seconds (3000 milliseconds)
  delay(3000);
  sendSMS();

  // Wait for 10 seconds before making a call (total 30 seconds delay)
  delay(10000);

  // Make a call
  makeCall();

  // Add a delay before making another call (if needed)
  delay(60000); // Delay for 1 minute (60000 milliseconds)
}

void sendSMS() {
  // AT command to set SMS mode
  gsmSerial.println("AT+CMGF=1");

  delay(1000);

  // Replace PHONE_NUMBER with the phone number you want to send the SMS to
  // Replace MESSAGE_CONTENT with the content of your message
  gsmSerial.print("AT+CMGS=\"0740390420\"\r");
  delay(1000);
  gsmSerial.print("GSM inawork");
  delay(100);
  gsmSerial.write(26); // ASCII code for Ctrl+Z
  delay(1000);
}

void makeCall() {
  // AT command to make a call
  gsmSerial.print("ATD+254740390420;");
  delay(1000);
  gsmSerial.println();
}

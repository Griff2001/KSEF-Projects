#include <SoftwareSerial.h>

#define GSM_TX 8  // Connect GSM module TX to NodeMCU D1
#define GSM_RX 9  // Connect GSM module RX to NodeMCU D2

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

int FloatSensor = 10; // Pin connected to the float sensor
int ledLow = 4;       // Pin connected to the green LED (low water level)
int ledMedium = 5;    // Pin connected to the yellow LED (medium water level)
int ledHigh = 13;     // Pin connected to the red LED (high water level)
int buttonState = 0;  // Reads float sensor status

unsigned long highWaterLevelStartTime = 0;
bool isHighWaterLevel = false;
bool smsSent = false;

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600); // Initialize GSM serial communication

  pinMode(FloatSensor, INPUT_PULLUP); // Arduino Internal Resistor 10K
  pinMode(ledLow, OUTPUT);
  pinMode(ledMedium, OUTPUT);
  pinMode(ledHigh, OUTPUT);

  Serial.println("Initializing GSM module...");

  delay(2000); // Give time for the GSM module to initialize

  // AT command to test if the GSM module is responding
  gsmSerial.println("AT");
}

void loop() {
  buttonState = digitalRead(FloatSensor);

  if (buttonState == HIGH) { // When float sensor is LOW, water level is low
    digitalWrite(ledLow, HIGH);    // Turn on green LED
    digitalWrite(ledMedium, HIGH);  // Turn off yellow LED
    digitalWrite(ledHigh, LOW);    // Turn off red LED
    Serial.println("WATER LEVEL - LOW");
    isHighWaterLevel = false;
    smsSent = false;
  } else if (buttonState == LOW) { // When float sensor is HIGH, water level is high
    digitalWrite(ledLow, LOW);    // Turn off green LED
    digitalWrite(ledMedium, LOW);  // Turn off yellow LED
    digitalWrite(ledHigh, HIGH);   // Turn on red LED
    Serial.println("WATER LEVEL - HIGH");

    if (!isHighWaterLevel) {
      isHighWaterLevel = true;
      highWaterLevelStartTime = millis();
    }

    if (millis() - highWaterLevelStartTime > 10000) {
      if (!smsSent) {
        sendSMS();
        smsSent = true;
      }
      makeCall();
    }
  }

  delay(1000);
}

void sendSMS() {
  // AT command to set SMS mode
  gsmSerial.println("AT+CMGF=1");

  delay(1000);

  // Replace PHONE_NUMBER with the phone number you want to send the SMS to
  // Replace MESSAGE_CONTENT with the content of your message
  gsmSerial.print("AT+CMGS=\"+254740390420\"\r");
  delay(1000);
  gsmSerial.print("High water level detected");
  delay(100);
  gsmSerial.write(26); // ASCII code for Ctrl+Z
  delay(1000);

  Serial.println("SMS sent");
}

void makeCall() {
  // AT command to make a call
  gsmSerial.print("ATD+254740390420;");
  delay(1000);
  gsmSerial.println();

  Serial.println("Call initiated");
}

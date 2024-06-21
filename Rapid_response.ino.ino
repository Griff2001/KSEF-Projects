#include <SoftwareSerial.h>

#define GSM_TX 6  // Connect GSM module TX to Arduino Pin 6
#define GSM_RX 7  // Connect GSM module RX to Arduino Pin 7

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

#define IN1  9
#define IN2  10
#define IN3  11
#define IN4  12

const int analogInPin = A0;
int sensorValue = 0;

int Steps = 0;
boolean Direction = true;
unsigned long last_time;
unsigned long currentMillis;
int steps_left = 4095;
long time;

boolean smsSent = false; // Flag to check if SMS has been sent already

void setup() {
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT);
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  Serial.begin(9600);
  gsmSerial.begin(9600); // Initialize GSM serial communication
}

void loop() {
  sensorValue = analogRead(analogInPin);
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\n");
  delay(1000);

  if ((sensorValue >= 340)) {
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    delay(5000);
    // Control stepper motor
    while (steps_left > 0) {
      currentMillis = micros();
      if (currentMillis - last_time >= 1000) {
        stepper(1);
        time = time + micros() - last_time;
        last_time = micros();
        steps_left--;
      }
    }
    Serial.println(time);
    Serial.println("Water level high. Activating motor!");
    // Send SMS if not already sent
    if (!smsSent) {
      sendSMS();
      smsSent = true; // Mark SMS as sent
    }
    delay(1000);
    // Make a call
    makeCall();
    delay(2000); // Delay to ensure the call and SMS are completed
    Direction = !Direction;
    steps_left = 4095;
  } 
  else {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    delay(100);
    // Reset SMS sent flag if water level is not high
    smsSent = false;
  }
}

void stepper(int xw) {
  for (int x = 0; x < xw; x++) {
    switch (Steps) {
      case 0:
        digitalWrite(IN1, LOW); 
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break; 
      case 1:
        digitalWrite(IN1, LOW); 
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, HIGH);
        break; 
      case 2:
        digitalWrite(IN1, LOW); 
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break; 
      case 3:
        digitalWrite(IN1, LOW); 
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break; 
      case 4:
        digitalWrite(IN1, LOW); 
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break; 
      case 5:
        digitalWrite(IN1, HIGH); 
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break; 
      case 6:
        digitalWrite(IN1, HIGH); 
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break; 
      case 7:
        digitalWrite(IN1, HIGH); 
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break; 
      default:
        digitalWrite(IN1, LOW); 
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break; 
    }
    SetDirection();
  }
} 

void SetDirection() {
  if (Direction == 1) {
    Steps++;
  }
  if (Direction == 0) {
    Steps--;
  }
  if (Steps > 7) {
    Steps = 0;
  }
  if (Steps < 0) {
    Steps = 7;
  }
}

void sendSMS() {
  Serial.println("Sending SMS...");
  // AT command to set SMS mode
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  // Replace PHONE_NUMBER with the phone number you want to send the SMS to
  // Replace MESSAGE_CONTENT with the content of your message
  gsmSerial.print("AT+CMGS=\"0717492935\"\r");
  delay(1000);
  gsmSerial.print("Water level high! Elevator activated.");
  delay(100);
  gsmSerial.write(26); // ASCII code for Ctrl+Z
  delay(1000);
}

void makeCall() {
  Serial.println("Making call...");
  // AT command to make a call
  gsmSerial.print("ATD+254717492935;");
  delay(1000);
  gsmSerial.println();
}

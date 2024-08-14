#include <SoftwareSerial.h>

#define GSM_TX 6  // Connect GSM module TX to Arduino Pin 6
#define GSM_RX 7  // Connect GSM module RX to Arduino Pin 7

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

#define IN1  9
#define IN2  10
#define IN3  11
#define IN4  12

const int analogInPin = A0;
const int analogInPin2 = A2; // Second water sensor
int sensorValue = 0;
int sensorValue2 = 0; // Value from second water sensor

int Steps = 0;
boolean Direction = true;
unsigned long last_time;
unsigned long currentMillis;
int steps_left = 4095;
long time;

boolean smsSent = false; // Flag to check if SMS has been sent already
boolean smsSent2 = false; // Flag to check if SMS for second sensor has been sent already
boolean callMade2 = false; // Flag to check if call for second sensor has been made already

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
  delay(1000); // Delay for 1 second before checking water level and activating motor
  sensorValue = analogRead(analogInPin);
  sensorValue2 = analogRead(analogInPin2);
  
  Serial.print("sensor1 = ");
  Serial.print(sensorValue);
  Serial.print("\n");
  Serial.print("sensor2 = ");
  Serial.print(sensorValue2);
  Serial.print("\n");

  if (sensorValue>= 340) {
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
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
    Serial.println("Hello, kindly be notified that the water level to the Dam is rising to a higher level!!");
    
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
else if (sensorValue >= 50 && sensorValue <= 290) {
    digitalWrite(3, HIGH); // Turn on LED connected to pin 3
    digitalWrite(2, LOW);  // Turn off LED connected to pin 2
    digitalWrite(5, LOW);
} 
else {
    digitalWrite(2, HIGH);  // Turn on LED connected to pin 2
    digitalWrite(3, LOW);   // Turn off LED connected to pin 3
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    delay(100);
    
    // Reset SMS sent flag if water level is not high
    smsSent = false;
}

 
   // Logic for second sensor
  if (sensorValue2 >= 100 && sensorValue2 < 180) {
    digitalWrite(5, HIGH);
    if (!smsSent2) {
      sendSMS2();
      smsSent2 = true; // Mark SMS as sent
    }
    callMade2 = false; // Reset call flag
  } 
  else if (sensorValue2 >= 200) {
    digitalWrite(5, HIGH);
    if (!callMade2) {
      makeCall2();
      callMade2 = true; // Mark call as made
    }
    smsSent2 = false; // Reset SMS flag
  } 
  else {
    smsSent2 = false; // Reset flags if water level is below threshold
    callMade2 = false;
     digitalWrite(5, LOW);
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
  gsmSerial.print("AT+CMGS=\"0728530962\"\r");
  delay(1000);
  gsmSerial.print("Hello, kindly be notified that the water flow towards the gauge is at a dangerous level!!");
  delay(100);
  gsmSerial.write(26); // ASCII code for Ctrl+Z
  delay(1000);
}

void sendSMS2() {
  Serial.println("Sending SMS for second sensor...");
  // AT command to set SMS mode
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  // Replace PHONE_NUMBER with the phone number you want to send the SMS to
  // Replace MESSAGE_CONTENT with the content of your message
  gsmSerial.print("AT+CMGS=\"0728530962\"\r");
  delay(1000);
  gsmSerial.print("Second sensor: Kindly be informed that the water level in the dam is at a higher level!!");
  delay(100);
  gsmSerial.write(26); // ASCII code for Ctrl+Z
  delay(1000);
}

void makeCall() {
  Serial.println("Making call...");
  // AT command to make a call
  gsmSerial.print("ATD+254728530962;");
  delay(1000);
  gsmSerial.println();
}

void makeCall2() {
  Serial.println("Making call for second sensor...");
  // AT command to make a call
  gsmSerial.print("ATD+254728530962;");
  delay(1000);
  gsmSerial.println();
}

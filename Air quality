#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Temperature Pins
#define outsideTempPin 3
#define insideTempPin 2
#define servoMotorPin 4
// Fan Pins
#define topFanPin 11
#define bottomFanPin 12

#define TOP_SERVO_PIN 6
#define BOTTOM_SERVO_PIN 7

// Gas sensor pin
const int gasSensorPin = A0;
const int BUZZER_PIN = 10;

#define DHTTYPE DHT11 // DHT 11

DHT insideTempSensor(outsideTempPin, DHTTYPE);
DHT outsideTempSensor(insideTempPin, DHTTYPE);

Servo topServo;
Servo bottomServo;

// Define LCD address and dimensions (typically 16x2)
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  Serial.begin(9600);
  outsideTempSensor.begin();
  insideTempSensor.begin();

  pinMode(topFanPin, OUTPUT);
  pinMode(bottomFanPin, OUTPUT);
 pinMode(BUZZER_PIN, OUTPUT);
  // Initialize fans to off state
  turnOffFans();

  // Initialize LCD
  lcd.init();
  lcd.backlight();

    // initialize servo
  topServo.attach(TOP_SERVO_PIN);
  bottomServo.attach(BOTTOM_SERVO_PIN);
}

/* CONTROLLING SERVO */
void openTopServo() {
  topServo.write(0);
}

void openBottomServo() {
  bottomServo.write(0);
}

void closeTopServo() {
  topServo.write(180);
}

void closeBottomServo() {
  bottomServo.write(180);
}

// Function to turn off fans
void turnOffFans() {
  digitalWrite(topFanPin, LOW); // Turn off top fan
  digitalWrite(bottomFanPin, LOW); // Turn off bottom fan
}

// Turn ON bottom Fan
void turnOnBottomFan(){
  digitalWrite(bottomFanPin, HIGH);
}

// Turn ON top Fan
void turnOnTopFan(){
  digitalWrite(topFanPin, HIGH);
}

// Turn on buzzer
void turnOffBuzzer() {
  digitalWrite(BUZZER_PIN, LOW);
}


// Function to read outside temperature
float readOutsideTemperature() {
  return outsideTempSensor.readTemperature(); // Return temperature in Celsius
}

// Function to read inside temperature
float readInsideTemperature() {
  return insideTempSensor.readTemperature(); // Return temperature in Celsius
}

// Function to read gas sensor value
int readGasSensor() {
  // Read the analog value from the gas sensor
  int sensorValue = analogRead(gasSensorPin);
  return sensorValue;
}

// Function to control fans based on temperature differentials and gas sensor value
void controlFans(float outsideTemp, float insideTemp) {
  int gasSensorValue = readGasSensor();
  int threshold = 300;
  
  // Only activate fans if gas sensor value is above threshold
  if (gasSensorValue > threshold) {
    if (outsideTemp > insideTemp) {
      // Outside temperature is greater than inside temperature
      // Activate the bottom fan
      closeBottomServo();
      openTopServo();
      delay(6000);
      turnOnBottomFan();
      // Turn top fan off
      digitalWrite(topFanPin, LOW);
    } else if (insideTemp > outsideTemp) {
      // Inside temperature is greater than outside temperature
      // Activate the top fan
            closeTopServo();
      openBottomServo();
      delay(6000);
      turnOnTopFan();
      // Turn bottom fan off
      digitalWrite(bottomFanPin, LOW);
    } else {
      // Inside and outside temperatures are equal
      // Turn off both fans
      turnOffFans();
    }
  } else {
    // Gas sensor value is not above threshold, turn off both fans
     closeTopServo();
      closeBottomServo();
    turnOffFans();
  }
}

void loop() {
  // Read gas sensor value and print to LCD after 2 seconds
  int gasSensorValue = readGasSensor();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas Level: ");
  lcd.print(gasSensorValue);
  delay(2000);

  // Read inside and outside temperatures and print to LCD
  float insideTemperature = readInsideTemperature();
  float outsideTemperature = readOutsideTemperature();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Outside: ");
  lcd.print(insideTemperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Inside: ");
  lcd.print(outsideTemperature);
  lcd.print("C");

  // Control fans based on temperature differentials and gas sensor value
  controlFans(outsideTemperature, insideTemperature);

  delay(2000); // Delay to allow the fan control to be noticeable
}

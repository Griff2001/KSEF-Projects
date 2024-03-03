// Blynk tempelates
#define BLYNK_TEMPLATE_ID "TMPL24u1nS5_d"
#define BLYNK_TEMPLATE_NAME "Agriculture"

// Library Imports
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
/*
Sensor Pins definitions
    - moisture : A0
    - relay module: D6
    - SCL: D1
    - SDA: D2
    - led/buzzer : D3
    - temperature : D4
    - pir motion: D5
    - laser : D7
    - laser receiever: D8
*/

// Message bot imports
#define BOTtoken "7104950327:AAH5cCvaVY_-5Y_6Iugm69oyADxwJESaz-A"
#define CHAT_ID "6375833499"

/* pin definitions*/
#define MOISTURE_PIN A0
#define RELAY_PUMP D6
#define LED_BUZZER D3
#define TEMPERATURE_PIN D4
#define PIR_PIN D5
#define LASER_PIN D7
#define LASER_RECEIVER_PIN D8

/*  Virtual pin for controlling blynk app*/
#define LED_VPIN V2
#define PUMP_VPIN V2
#define HUMIDITY_VPIN V2

// Blynk authentication token and WiFi credentials
char auth[] = "x5UKdAPqJM-kocN5MrT3FZb8Abyf-uLp";
char ssid[] = "Wegner";
char pass[] = "Wegner123!";

// Message bot initialiaztion
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Function prototypes
void initializePins();
void initializeBlynk();
void initializeLCD();
void setupWiFi();
void setupTelegramBot();
void fetchMoistureLevel();
void sendMoistureToBlynk(int moistureLevel);
void printMoistureOnLCD(int moistureLevel);
void controlPump(int moistureLevel);
void sendSMSAlert();

void setup()
{
    Serial.begin(9600);
    initializePins();
    initializeBlynk();
    initializeLCD();
}

void loop()
{
    // Get laser data
    bool value = digitalRead(LASER_RECEIVER_PIN);
    Serial.println(value);

    // Monitor birds
    detectBirdUsingLaser(value);
    Blynk.run();
}

// Function definitions

void initializePins()
{
    pinMode(RELAY_PUMP, OUTPUT);
    pinMode(LED_BUZZER, OUTPUT);
    pinMode(LASER_PIN, OUTPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(LASER_RECEIVER_PIN, INPUT);
}

void initializeBlynk()
{
    Blynk.begin(auth, ssid, pass);
}

void initializeLCD()
{
    LiquidCrystal_I2C lcd(0x27, 16, 2);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Agriculture");
}

void setupWiFi()
{
    Serial.println("Connecting to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass); // Change 'password' to 'pass'
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWiFi connected.");
}

void setupTelegramBot()
{
    Serial.println("Setting up Telegram bot...");
    configTime(0, 0, "pool.ntp.org");
    client.setTrustAnchors(&cert);
    Serial.println("Telegram bot ready.");
}

void getTemperature()
{
    // Code to read temperature and print on LCD
}

void getHumidity()
{
    // Code to read humidity and print on LCD
}

void handleMotionDetected()
{
    // Code to handle motion detection
}

void handleMotionStopped()
{
    // Code to handle motion stopped
}

// Code to control LED and buzzer based on state
void controlLEDAndBuzzer(bool state) {
    if (!state) {
        digitalWrite(LED_BUZZER, LOW);
    }
}


// Code to detect bird using laser
void detectBirdUsingLaser(bool value)
{
    if (value == true)
    {
        digitalWrite(LED_BUZZER, HIGH);
        bot.sendMessage(CHAT_ID, "Warning!", "");
    }
    else if (value == false)
    {
        digitalWrite(LED_BUZZER, LOW);
    }
}


/* HANDLING MOISTURE */

void fetchMoistureLevel() {
    int moistureLevel = analogRead(MOISTURE_PIN);
    sendMoistureToBlynk(moistureLevel);
    printMoistureOnLCD(moistureLevel);
    controlPump(moistureLevel);
}

void controlPump(int moistureLevel) {
    if (moistureLevel < THRESHOLD_MOISTURE_LEVEL) {
        digitalWrite(RELAY_PUMP, HIGH); // Turn on the pump
        sendSMSAlert();
    } else {
        digitalWrite(RELAY_PUMP, LOW); // Turn off the pump
    }
}

void sendMoistureToBlynk(int moistureLevel) {
    Blynk.virtualWrite(MOISTURE_VPIN, moistureLevel);
}

void printMoistureOnLCD(int moistureLevel) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Moisture: ");
    lcd.print(moistureLevel);
}


/* HANLING TEMPERATURE*/
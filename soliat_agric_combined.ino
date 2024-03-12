#define BLYNK_TEMPLATE_ID "TMPL24u1nS5_d"
#define BLYNK_TEMPLATE_NAME "Agriculture"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Network Configuration
char auth[] = "x5UKdAPqJM-kocN5MrT3FZb8Abyf-uLp";
char ssid[] = "Wegner";
char pass[] = "Wegner123!";

#define BOTtoken "7104950327:AAH5cCvaVY_-5Y_6Iugm69oyADxwJESaz-A"
#define CHAT_ID "6375833499"

/* Virtual Pins*/
#define HUMIDITY_VPIN V2
#define TEMPERATURE_VPIN V1
#define PIR_VPIN V4
#define PUMP_CONTROL_VPIN V3

// pin declarations
#define LED_BUZZER D6
#define LReceiver D8
#define Laser D7
#define TEMPERATURE_PIN D3
#define PIR_PIN D5
#define PUMP_PIN D2 
#define MOISTURE_PIN D1 

/* Global Variables */
#define DHT_TYPE DHT11
DHT dht(TEMPERATURE_PIN, DHT_TYPE);

// telegram sms
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

bool motionDetected = false;
bool automaticMode = true; // Flag to indicate automatic mode

void configurations() {
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");// get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  pinMode(LReceiver, INPUT);
  pinMode(LED_BUZZER, OUTPUT);
  pinMode(Laser, OUTPUT);
  digitalWrite(Laser, HIGH);
  pinMode(PIR_PIN, INPUT);

  Blynk.begin(auth, ssid, pass);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "System is Ready", "");
  delay(1000);
}

/* functions initialization */
void detects_birds();
void sendSMSAlert(const char *messageContent);
void readTemperatureAndHumidity();
void sendTemperatureToBlynk(float temperatureLevel);
void sendHumidityToBlynk(float humidityLevel);
void detectMotion();
void handleMotionDetected();
void handleMotionStopped();

void setup() {
  configurations();
  pinMode(PUMP_PIN, OUTPUT); 
  pinMode(MOISTURE_PIN, INPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();
  readTemperatureAndHumidity();
  detects_birds();
  detectMotion();
  if (automaticMode) {
    int moistureLevel = map(analogRead(MOISTURE_PIN), 0, 1023, 0, 100); // Read the moisture level and map it to a range of 0-100

    if (moistureLevel < 80) { // If moisture level is above 80
      // Turn the pump off
      digitalWrite(PUMP_PIN, LOW);
    } else if (moistureLevel > 70) { // If moisture level is below 70
      // Turn the pump on
      digitalWrite(PUMP_PIN, HIGH);
    }
  }
  delay(1000); // Wait for 1 second before checking moisture level again
}

// send sms to telegram
void sendSMSAlert(const char *messageContent)
{
  bot.sendMessage(CHAT_ID, messageContent, "");
}

void detects_birds() {
  bool value = digitalRead(LReceiver);
  if (value == 1) {
    digitalWrite(LED_BUZZER, HIGH);
    const char *message = "Warning: Birds In the farm!";
    sendSMSAlert(message);
  } else { // Turn off buzzer if no obstacle detected
    digitalWrite(LED_BUZZER, LOW);
  }
}

/* ------------------------ HANDLE TEMPERATURE -----------------------------*/
void readTemperatureAndHumidity()
{
  float temperatureLevel = dht.readTemperature();
  float humidityLevel = dht.readHumidity();

  if (!isnan(temperatureLevel))
  {
    sendTemperatureToBlynk(temperatureLevel);
  }

  if (!isnan(humidityLevel))
  {
    sendHumidityToBlynk(humidityLevel);
  }
}

void sendTemperatureToBlynk(float temperatureLevel)
{
  Blynk.virtualWrite(TEMPERATURE_VPIN, temperatureLevel);
}

void sendHumidityToBlynk(float humidityLevel)
{
  Blynk.virtualWrite(HUMIDITY_VPIN, humidityLevel);
}

/* ------------------HANDLING MOTION AND BULGARY--------------- */

void detectMotion()
{
  motionDetected = digitalRead(PIR_PIN);
  if (motionDetected) {
    handleMotionDetected();
  } else {
    handleMotionStopped();
  }
}

void handleMotionDetected()
{
  Blynk.virtualWrite(PIR_VPIN, 1);
}

void handleMotionStopped()
{
   Blynk.virtualWrite(PIR_VPIN, 0); 
}

BLYNK_WRITE(PUMP_CONTROL_VPIN){
  digitalWrite(PUMP_PIN, param.asInt());
  automaticMode = false; // Switch to manual mode when Blynk control is activated
}

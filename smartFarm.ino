// Blynk tempelates
#define BLYNK_TEMPLATE_ID "TMPL24u1nS5_d"
#define BLYNK_TEMPLATE_NAME "Agriculture"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>



// Network Configuration
char auth[] = "x5UKdAPqJM-kocN5MrT3FZb8Abyf-uLp";
char ssid[] = "Wegner";
char pass[] = "Wegner123!";

/* Message sending */
#define BOTtoken "7104950327:AAH5cCvaVY_-5Y_6Iugm69oyADxwJESaz-A"
#define CHAT_ID "6375833499"

// Message bot initialiaztion
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

/* Virtual Pins*/
#define HUMIDITY_VPIN V2
#define TEMPERATURE_VPIN V1
#define PUMP_CONTROL_VPIN V3

/*  Pins definitions*/
#define TEMPERATURE_PIN D3
#define PUMP_PIN D4
#define MOISTURE_PIN A0
#define PIR_PIN D5
#define LED_BUZZER D3

/* Global Variables */
#define DHT_TYPE DHT11
DHT dht(TEMPERATURE_PIN, DHT_TYPE);
bool automaticMode = true;

/* function Prototypes*/
void setupWiFi();
void setupTelegramBot();
void sendSMSAlert(const char *messageContent);

void readTemperatureAndHumidity();
void sendTemperatureToBlynk(float temperatureLevel);
void sendHumidityToBlynk(float humidityLevel);
void controlPump();


void setup()
{
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(MOISTURE_PIN, INPUT);
    dht.begin();
}

void loop()
{
    Blynk.run();
    readTemperatureAndHumidity();
    controlPump();
    delay(1000);
}

/* Files Setup */
void setupWiFi()
{
    Serial.println("Connecting to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass); 
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

void sendSMSAlert(const char *messageContent)
{
    bot.sendMessage(CHAT_ID, messageContent, "");
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

/* ------------------------ HANDLE PUMP -----------------------------*/
void controlPump()
{
    Blynk.run();
    if (automaticMode)
    {
        int moistureLevel = map(analogRead(MOISTURE_PIN), 0, 1023, 0, 100);
        Serial.print("Moisture Level: ");
        Serial.println(moistureLevel);

        if (moistureLevel > 80)
        {
            digitalWrite(PUMP_PIN, LOW);
            Serial.println("Pump OFF");

            const char *message = "PUMPING WATER: Moisture Low!";
            sendSMSAlert(message);
        }
        else if (moistureLevel < 70)
        {
            digitalWrite(PUMP_PIN, HIGH);
            Serial.println("Pump ON");

            const char *message = "Finished Pumping";
            sendSMSAlert(message);
        }
    }
}

BLYNK_WRITE(PUMP_CONTROL_VPIN)
{
    digitalWrite(PUMP_PIN, param.asInt());
    automaticMode = false;
    const char *message = "PUMPING WATER: Manually Triggered";
    sendSMSAlert(message);
}

/* ------------------HANDLING MOTION AND BULGARY--------------- */

void detectMotion()
{
    bool motionDetected = digitalRead(PIR_PIN);

    if (motionDetected)
    {
        handleMotionDetected();
    }
    else
    {
        handleMotionStopped();
    }
}

void handleMotionDetected()
{
    digitalWrite(LED_BUZZER, HIGH);

    const char *message = "SOMEONE IN THE FARM";
    sendSMSAlert(message);
}

void handleMotionStopped()
{
    digitalWrite(LED_BUZZER, LOW);
}

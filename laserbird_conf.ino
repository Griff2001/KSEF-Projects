#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "Wegner";
const char* password = "Wegner123!";


#define BOTtoken "7104950327:AAH5cCvaVY_-5Y_6Iugm69oyADxwJESaz-A"
#define CHAT_ID "6375833499"
#define LED D4
#define Buzzer D3
#define LReceiver D8
#define Laser D7

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


void configurations() {
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");// get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  pinMode(LReceiver, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(Laser, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Laser, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  bot.sendMessage(CHAT_ID, "System is Ready", "");
  delay(1000);
}


void detects_birds();
void sendSMSAlert(const char *messageContent);

void setup() {
  configurations();
}

void loop() {
  detects_birds();
}

void sendSMSAlert(const char *messageContent)
{
  bot.sendMessage(CHAT_ID, messageContent, "");
}


void detects_birds() {
  bool value = digitalRead(LReceiver);
  Serial.println(value);
  if (value == 1) {
    digitalWrite(LED, HIGH);
    digitalWrite(Buzzer, HIGH);
    const char *message = "Warning: Birds In the farm!";
    sendSMSAlert(message);
  } else if (value == 0) {
    digitalWrite(LED, LOW);
    digitalWrite(Buzzer, LOW);
  }
}

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
#define LReceiver D5
#define Laser D1

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


void setup() {
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

void loop() {
  bool value = digitalRead(LReceiver);
  Serial.println(value);
  if (value == 1) {
    digitalWrite(LED, HIGH);
    digitalWrite(Buzzer, HIGH);
    bot.sendMessage(CHAT_ID, "Warning!", "");
  } else if (value == 0) {
    digitalWrite(LED, LOW);
    digitalWrite(Buzzer, LOW);

  }
}

// Blynk templates
#define BLYNK_TEMPLATE_ID "TMPL24u1nS5_d"
#define BLYNK_TEMPLATE_NAME "Agriculture"

#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
#include <DHT.h>


// Blynk authentication token and WiFi credentials
char auth[] = "x5UKdAPqJM-kocN5MrT3FZb8Abyf-uLp";
char ssid[] = "Wegner";
char pass[] = "Wegner123!";

// Virtual pin for controlling Blynk app
#define HUMIDITY_VPIN V2
#define TEMPERATURE_VPIN V1


// Pin connected to the DHT sensor
#define DHT_PIN 2
#define DHT_TYPE DHT11



DHT dht(DHT_PIN, DHT_TYPE);

// Function prototypes
void readTemperatureAndHumidity();
void sendTemperatureToBlynk(float temperatureLevel);
void sendHumidityToBlynk(float humidityLevel);

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
}

void loop() {
  Blynk.run();
  readTemperatureAndHumidity();
  delay(1000);
}

// Read temperature and humidity from the DHT sensor
void readTemperatureAndHumidity() {
  float temperatureLevel = dht.readTemperature();
  float humidityLevel = dht.readHumidity();

  if (!isnan(temperatureLevel)) {
    sendTemperatureToBlynk(temperatureLevel);
  }

  if (!isnan(humidityLevel)) {
    sendHumidityToBlynk(humidityLevel);
  }
}

// Send temperature data to Blynk
void sendTemperatureToBlynk(float temperatureLevel) {
  Blynk.virtualWrite(TEMPERATURE_VPIN, temperatureLevel);
}

// Send humidity data to Blynk
void sendHumidityToBlynk(float humidityLevel) {
  Blynk.virtualWrite(HUMIDITY_VPIN, humidityLevel);
}

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL2akBAfpiT"
#define BLYNK_TEMPLATE_NAME "Noise Monitoring"
#define BLYNK_AUTH_TOKEN "O3TzQ4JLT1GW8I3XQhV-uu2bSNiwbUX9"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>


/* Comment this out to disable prints and save space */
// #define BLYNK_PRINT Serial

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wegner";
char pass[] = "Wegner123!";

int thresholdValue;
int buzzerPin = D3;

BlynkTimer timer;

// GSM Module Configuration
#define GSM_TX D6  // Connect GSM module TX to NodeMCU D1
#define GSM_RX D5  // Connect GSM module RX to NodeMCU D2

SoftwareSerial gsmSerial(GSM_TX, GSM_RX);

int soundLevel() {
  int analogValue = analogRead(A0);
  Serial.println(analogValue);
  return analogValue;
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  thresholdValue = param.asInt();
  // Update state
  // Blynk.virtualWrite(V1, value);
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  int sound = soundLevel();
  if (sound > thresholdValue) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    Serial.println(thresholdValue);

    // Send SMS after detecting high noise
    sendSMS();

    // Wait for 25 seconds before making a call (total 30 seconds delay)
    delay(10000);

    // Make a call
    makeCall();
  } else {
    digitalWrite(buzzerPin, LOW);
  }
  Blynk.virtualWrite(V2, sound);
}

void sendSMS() {
  // AT command to set SMS mode
  gsmSerial.println("AT+CMGF=1");
  delay(1000);

  // Replace PHONE_NUMBER with the phone number you want to send the SMS to
  // Replace MESSAGE_CONTENT with the content of your message
  gsmSerial.print("AT+CMGS=\"0740390420\"\r");
  delay(1000);
  gsmSerial.print("GSM inawork");
  delay(100);
  gsmSerial.write(26); // ASCII code for Ctrl+Z
  delay(1000);
}

void makeCall() {
  // AT command to make a call
  gsmSerial.print("ATD+254740390420;"); 
  delay(1000);
  gsmSerial.println();
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  // Change Web Link Button message to "Congratulations!"
  // Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  // Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  // Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void setup() {
  // Debug console
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);

  // GSM module initialization
  gsmSerial.begin(9600); // Initialize GSM serial communication
  Serial.println("Initializing GSM module...");
  delay(2000); // Give time for the GSM module to initialize

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
}

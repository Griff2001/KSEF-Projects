/*********************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *********************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL2akBAfpiT"
#define BLYNK_TEMPLATE_NAME "Noise Monitoring"
#define BLYNK_AUTH_TOKEN "O3TzQ4JLT1GW8I3XQhV-uu2bSNiwbUX9"

/* Comment this out to disable prints and save space */
// #define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wegner";
char pass[] = "Wegner123!";

int thresholdValue;
int buzzerPin=D3;

BlynkTimer timer;
int soundLevel(){

  int analogValue=analogRead(A0);
  Serial.println(analogValue);

  return analogValue;

  
}
// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int thresholdValue = param.asInt();

  // Update state
  // Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  // Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  // Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  // Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  int sound=soundLevel();
  if (sound>thresholdValue){
    digitalWrite(buzzerPin, LOW);
  }
  else{
     digitalWrite(buzzerPin, HIGH);
  }
  Blynk.virtualWrite(V2, sound);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  
}

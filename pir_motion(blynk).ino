// virtual Pins definition
#define PIR_VPIN V4
#define PIR_PIN D5

// Initialize Blynk with authentication token and WiFi credentials
void setup() {
  pinMode(PIR_PIN, INPUT);
  
}

// Blynk loop
void loop() {
  Blynk.run();
  detectMotion();
}


void detectMotion() {
  bool motionDetected = digitalRead(PIR_PIN);
  if (motionDetected) {
    handleMotionDetected();
  } else {
    handleMotionStopped();
  }
}


void handleMotionDetected() {
  Blynk.virtualWrite(PIR_VPIN, 1);
}


void handleMotionStopped() {
  Blynk.virtualWrite(PIR_VPIN, 0); 
}

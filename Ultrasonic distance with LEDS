const int trig = 12;
const int echo = 13;

const int LED1 = 8;
const int LED2 = 7;
const int LED3 = 6;
const int LED4 = 5;
const int LED5 = 4;
const int LED6 = 3;
const int LED7 = 2;
const int LED8 = 1;
const int LED9 = 10; // Adjusted LED9 to pin 10
const int LED10 = 9; // Added LED10

int duration = 0;
int distance = 0;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(LED9, OUTPUT); // Added LED9
  pinMode(LED10, OUTPUT); // Added LED10
  
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) / 28.5;
  Serial.println(distance);

  // Turn on LEDs based on distance
  digitalWrite(LED1, distance <= 7 ? HIGH : LOW);
  digitalWrite(LED2, distance <= 14 ? HIGH : LOW);
  digitalWrite(LED3, distance <= 21 ? HIGH : LOW);
  digitalWrite(LED4, distance <= 28 ? HIGH : LOW);
  digitalWrite(LED5, distance <= 35 ? HIGH : LOW);
  digitalWrite(LED6, distance <= 42 ? HIGH : LOW);
  digitalWrite(LED7, distance <= 49 ? HIGH : LOW);
  digitalWrite(LED8, distance <= 56 ? HIGH : LOW);
  digitalWrite(LED9, distance <= 63 ? HIGH : LOW); // Adjusted LED9
  digitalWrite(LED10, distance <= 70 ? HIGH : LOW); // Added LED10
}

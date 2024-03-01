//the code is meant to fire laser, sich that when birds cross the farm, it turn buzzer and LED to scare them away. 

int laserPin =5;//D1
int buzPin = 4;//D2

void setup(){
Serial.begin(9600); // initialize serial comminication at 9600 BPS

 pinMode(laserPin, OUTPUT);
 pinMode(buzPin, OUTPUT);
digitalWrite(buzPin, LOW);
digitalWrite(laserPin, HIGH);
 
}
void loop (){
int sensorValue= analogRead(A0); //read the input on analog pin 0
Serial.println(sensorValue);

  if(sensorValue > 1000){
    Serial.println('>');
    digitalWrite(buzPin, LOW);
    delay(1000);
  }

  else{
    digitalWrite(buzPin, LOW);
    delay(1000);
  }
}

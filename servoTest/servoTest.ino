#include <ESP32Servo.h>

Servo myServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myServo.attach(12);
}

void loop() {
  while(Serial.available()){
    long x = Serial.parseInt();
    myServo.write(x);
  }


  // put your main code here, to run repeatedly:

}

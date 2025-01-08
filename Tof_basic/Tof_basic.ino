// #include <String>
#include <ESP32Servo.h>
Servo myservo;

void setup() {
  myservo.attach(12);
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    while (Serial.available()) {
      long x = Serial.parseInt();
      myservo.write(x);
      Serial.println(x);
    }
  }

}

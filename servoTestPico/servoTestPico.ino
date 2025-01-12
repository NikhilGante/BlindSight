// #include <ESP32Servo.h>
#include <Servo.h>

Servo myServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myServo.attach(D12);
}

void loop() {
  while(Serial.available()){
    long x = Serial.parseInt();
    myServo.write(x);
    Serial.print("hi ");
    Serial.println(x);
  }


  // put your main code here, to run repeatedly:

}

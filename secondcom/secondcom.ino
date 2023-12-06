#include <Servo.h>
#include <SoftwareSerial.h>
const byte rxPin = 2;
const byte txPin = 3;
int i;
// Set up a new SoftwareSerial object
SoftwareSerial mySerial (rxPin, txPin);
Servo my_servo;

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
  my_servo.attach(A0);
}
int s;
void loop() {
    i=analogRead(A1);
    i=map(i,0,1023,0,4);
    if (mySerial.available()>0){
        s=mySerial.read();
    }
    Serial.println(s);
    if (s==123){

      switch (i) {
        case 0:
          my_servo.write(0);
          delay(0);
          break;
        case 1:
          my_servo.write(0);
          delay(2000);
          my_servo.write(90);
          delay(2000);
          break;
        case 2:
          my_servo.write(0);
          delay(1500);
          my_servo.write(90);
          delay(1500);
          break;
        case 3:
          my_servo.write(0);
          delay(1200);
          my_servo.write(90);
          delay(1200);
          break;
        case 4:
          my_servo.write(0);
          delay(1000);
          my_servo.write(90);
          delay(1000);
          break;
      }
    }
}
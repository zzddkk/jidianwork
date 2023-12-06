#include <SoftwareSerial.h>
//#include <TimerOne.h>
//#include <Timer2ServoPwm.h>
#include <TM1637.h>
#include <MsTimer2.h>
//#include <Servo.h>
#define TimeCLK 7
#define TimeDIO 12
int mins=0;
int secs=0;
int i;
int counttime=5*60;


const byte rxPin = 2;
const byte txPin = 3;
// Set up a new SoftwareSerial object
SoftwareSerial mySerial (rxPin, txPin);

//Servo my_servo;
//Timer2Servo my_servo;
TM1637 tm1637(TimeCLK, TimeDIO);
void setup() {
  // put your setup code here, to run once:
  //my_servo.attach(A0);
  mySerial.begin(9600);
  tm1637.init();
  tm1637.set(2);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  MsTimer2::set(1000, printtime); // 1000ms period
  MsTimer2::start();
  //Timer1.initialize( 1000 ); // 初始化,   interval 以 micro sec 为单位
  //Timer1.attachInterrupt( printtime );
}


void loop() {
    // put your main code here, to run repeatedly:
  //Timer1.stop();
  //i=analogRead(A1);
  //i=map(i,1,1023,0,5);
  //Serial.print(i);
  //servo_run();
  //Timer1.restart();
  mySerial.write(123);
  delay(2000);
}



void printtime(){
  if (counttime > 0) {
    counttime--;
    int minutes = counttime / 60;
    int seconds = counttime % 60;
    tm1637.display(1,minutes);
    tm1637.display(2,seconds/10);
    tm1637.display(3,seconds%10);
  }
}
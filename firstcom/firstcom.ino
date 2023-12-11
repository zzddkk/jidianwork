#include "HX711.h"
#include <SoftwareSerial.h>
//#include <TimerOne.h>
//#include <Timer2ServoPwm.h>
#include <TM1637.h>
#include <MsTimer2.h>
//#include <Servo.h>
#define TimeCLK 8
#define TimeDIO 9

#define VOLUME          0x01    //设置音量
#define dataPin 12 // 音频模块 data引脚 D12
#define MUSIC_SELECT    0x0B    //选曲确认

int mins=0;
int secs=0;
int i;
int counttime=20;
long reading=0;
int weight_flag=0;
int voice_flag=1;

const byte rxPin = 2;
const byte txPin = 3;
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;
// Set up a new SoftwareSerial object
SoftwareSerial mySerial (rxPin, txPin);
HX711 scale;
//Servo my_servo;
//Timer2Servo my_servo;
TM1637 tm1637(TimeCLK, TimeDIO);
void setup() {
  // put your setup code here, to run once:
  //my_servo.attach(A0);
  pinMode(dataPin, OUTPUT);
  sendData(3);
  sendData(0);
  sendData(VOLUME); // 设置音量等级
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.begin(57600);
  mySerial.begin(9600);
  tm1637.init();
  tm1637.set(2);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  MsTimer2::set(1000, printtime); // 1000ms period
  theresetprinttime();
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

  if (scale.is_ready()) {
    reading = scale.get_value(10);
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  } else {
    reading=0;
  }
  if (reading>1000){
        if (weight_flag==0){
        CPlay(2,0x02, MUSIC_SELECT);  // 选择 m 曲目播放
        MsTimer2::start();
        }
        if (counttime==0){
          theresetprinttime();
          weight_flag=0;
        }
        Serial.println(weight_flag);
        mySerial.write(weight_flag);
        weight_flag=1;

  }
  if ((reading<1000 && weight_flag==1) || (counttime==0 && weight_flag==0)){
    weight_flag=0;
    mySerial.write(weight_flag);
    CPlay(2, 0x01, MUSIC_SELECT);  // 选择 m 曲目播放
    tm1637_reset();
    MsTimer2::stop();
    theresetprinttime();

  }
  //Serial.println(weight_flag);
    delay(1000);
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


void theresetprinttime(){
    tm1637.display(1,0);
    tm1637.display(2,2);
    tm1637.display(3,0);
}


/*
   多参数功能函数
    参数：cNotes - 连码语音数量
         uAddr - 语音地址
*/
void CPlay(byte cNotes, ...) {
  va_list ap;
  unsigned int uAddr;
  va_start(ap, cNotes);
  while (cNotes > 0) {
    uAddr = va_arg(ap, unsigned int);
    sendData(uAddr);
    cNotes--;
  }
  va_end(ap);
}

void sendData(int addr) {
  digitalWrite(dataPin , HIGH);
  delay(1);           // >1ms
  digitalWrite(dataPin , LOW);
  delay(3);           // >3ms
  for (int i = 0; i < 8; i++) {
    digitalWrite(dataPin, HIGH);
    if (addr & 1) {
      delayMicroseconds(1200); // >1200us
      digitalWrite(dataPin, LOW);
      delayMicroseconds(400);  // >400us
    } else {
      delayMicroseconds(400);  // >400us
      digitalWrite(dataPin , LOW);
      delayMicroseconds(1200); // >1200us
    }
    addr >>= 1;
  }
  digitalWrite(dataPin, HIGH);
}

void tm1637_reset(){
  counttime= 20;
  delay(100);
}
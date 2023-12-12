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
int counttime=30;//定时时间设置
long reading=0;
int weight_flag=0;//用于记录前一个状态

const byte rxPin = 2;
const byte txPin = 3;
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;
// Set up a new SoftwareSerial object
SoftwareSerial mySerial (rxPin, txPin);
HX711 scale;      //对HX711压力传感器实例声明
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
  Serial.begin(57600);  //  硬串口波特率设置用于调试
  mySerial.begin(9600); //软串口波特兰设置用于传输flag
  tm1637.init();  //定时器初始化
  tm1637.set(2);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;亮度模式设置
  MsTimer2::set(1000, printtime); // 1000ms period，定时器设置，每1000ms调用一次回调函数
  theresetprinttime();//四位显示器初始显示
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

  if (scale.is_ready())//检测压力传感器是否接收到数据 {
    reading = scale.get_value(10);//读取10ms内的数据平均值，避免因为单个的异常点影响结果
    Serial.print("HX711 reading: ");//用于检测调试
    Serial.println(reading);
  } else {
    reading=0;//reading==0则传感器没有收到任何值
  }
<<<<<<< HEAD
  if (reading>1000){//重物放到了压力传感器上
        if (weight_flag==0)//前一个状态上没有重物{
        //CPlay(4,7,7 , MUSIC_SELECT);  // 选择 m 曲目播放
          MsTimer2::start();//定时器启动
=======
  if (reading>1000){
        if (weight_flag==0){
        CPlay(2,0x02, MUSIC_SELECT);  // 选择 m 曲目播放
        MsTimer2::start();
>>>>>>> ae4c252309113944a36d2ead88793f48efd5b947
        }
        if (counttime==0)//如果存在重物而且定时器计时完毕{
          MsTimer2::stop();
          theresetprinttime();//四位显示器显示初始状态
          weight_flag=0;//将此时刻假设为没有重物的状态以此实现舵机的正确运行
        }
        Serial.println(weight_flag);//发送重物标志
        mySerial.write(weight_flag);
        weight_flag=1;//将此时刻记录为存在重物,这里才是实际状态的记录

  }
  if ((reading<1000 && weight_flag==1)){
    /*
    如果此时刻没有重物而且前一时刻有重物
    */
    weight_flag=0;//重物离去
    mySerial.write(weight_flag);
<<<<<<< HEAD
    CPlay(30, 1, MUSIC_SELECT);  // 选择“语音”播放
    MsTimer2::stop();//定时器停止
    theresetprinttime();//四位显示器初始显示
=======
    CPlay(2, 0x01, MUSIC_SELECT);  // 选择 m 曲目播放
    tm1637_reset();
    MsTimer2::stop();
    theresetprinttime();
>>>>>>> ae4c252309113944a36d2ead88793f48efd5b947

  }
  //Serial.println(weight_flag);
    delay(1000);//避免发送过快冲爆缓存
}



void printtime(){//定时器回调函数
  if (counttime > 0) {
    counttime--;
    int minutes = counttime / 60;
    int seconds = counttime % 60;
    tm1637.display(1,minutes);
    tm1637.display(2,seconds/10);
    tm1637.display(3,seconds%10);
  }
}


void theresetprinttime(){//初始显示
    tm1637.display(1,0);
    tm1637.display(2,3);
    tm1637.display(3,0);
    counttime= 30;
}


/*
   多参数功能函数
    参数：cNotes - 连码语音数量
         uAddr - 语音地址
*/
void CPlay(byte cNotes, ...) {//音乐播放函数
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

void sendData(int addr) {//将地址发送给语音传感器
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
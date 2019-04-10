/*
* 下位机 1.1 beta
* by czl & robin
*这是一个测试版本，用于后期修改出错时补救
* 2019/04/10
*/
int val1, val2;
int CompassAngle0, CompassAngle1, CompassAngle2;
int delta(int val1, int val2);
int zhinan();
#include <SoftwareSerial.h>//软串口通信库函数

SoftwareSerial mySerial(8, 9); //指南针模块的RX接11脚 TX接10脚

                 //全局函数声明
void forward(int);//前进
void backward(int);//后退
void left(int);//向左
void right(int);//向右
void stoping(int);//刹车
void clearing();//关闭使能端口
        //--控制车运动的函数括号里为持续的时间，单位毫秒--

        //arduino引脚声明
byte rightwheel0 = 5;
byte rightwheel1 = 7;
byte leftwheel0 = 4;
byte leftwheel1 = 2;
byte righten = 6;//使能端（右轮）
byte leften = 3;//使能端（左轮）

        //全局变量声明

        //主函数开始
void setup() {
  for (byte i = 2; i <= 7; i++)
    pinMode(i, OUTPUT);//定义接口
  Serial.begin(9600);//与电脑硬串口波特率
  mySerial.begin(9600);//软串口波特率
}

void loop() {
  clearing();//关闭使能端口作为初始化
  delay(3000);
  Serial.print("第一次测量得到：");
  int val1 = zhinan();
  right(1000);
  clearing();
  Serial.print("第二次测量得到：");
  int val2 = zhinan();
  while (delta(val1, val2) <= 85) {
    right(100);
    clearing();
    val2 = zhinan();
  }
  clearing();
  delay(5000);
}

//封装函数定义
void forward(int microseconds) {
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子正转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转

  delay(microseconds);
}

void backward(int microseconds) {
  digitalWrite(leftwheel0, HIGH);
  digitalWrite(leftwheel1, LOW);
  analogWrite(leften, 255);
  //左边轮子反转

  digitalWrite(rightwheel0, HIGH);
  digitalWrite(rightwheel1, LOW);
  analogWrite(righten, 255);
  //右边轮子反转

  delay(microseconds);
}

void left(int microseconds) {
  digitalWrite(leftwheel0, HIGH);
  digitalWrite(leftwheel1, LOW);
  analogWrite(leften, 255);
  //左边轮子反转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转

  delay(microseconds);
}

void right(int microseconds) {
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子正转

  digitalWrite(rightwheel0, HIGH);
  digitalWrite(rightwheel1, LOW);
  analogWrite(righten, 255);
  //右边轮子反转

  delay(microseconds);
}

void stoping(int microseconds) {
  digitalWrite(leftwheel0, HIGH);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子反转

  digitalWrite(rightwheel0, HIGH);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转
  delay(150);//刹车用时150ms

  clearing();//关闭使能端
  delay(microseconds);
}

void clearing(){
  analogWrite(leften, 0);
  analogWrite(righten, 0);
}


int delta(int val1, int val2) {
  if (abs(val1 - val2)>180)
    return abs(abs(val1 - val2) - 360);
  else
    return abs(val1 - val2);
}

int zhinan()
{
loop:mySerial.write(byte(0x31));//发送指令
  delay(200);
  if (mySerial.available())//共返回8个字节
  {
    byte Byte0 = mySerial.read();//开始读取缓冲区里的字节
    byte Byte1 = mySerial.read();//byte型变量为字节型
    byte Byte2 = mySerial.read();
    byte Byte3 = mySerial.read();
    byte Byte4 = mySerial.read();
    byte Byte5 = mySerial.read();
    byte Byte6 = mySerial.read();
    byte Byte7 = mySerial.read();
    CompassAngle1 = (Byte2 - 0x30) * 100 + (Byte3 - 0x30) * 10 + (Byte4 - 0x30);//算出角度
    //Serial.println(CompassAngle1);
    if (CompassAngle1<0 || CompassAngle1>360)goto loop;
  }

    //while (Serial.available())
      //byte Byte0 = Serial.read();//清空缓冲区

    mySerial.write(byte(0x31));//发送指令
  delay(200);
  if (mySerial.available())//共返回8个字节
  {
    byte Byte0 = mySerial.read();//开始读取缓冲区里的字节
    byte Byte1 = mySerial.read();//byte型变量为字节型
    byte Byte2 = mySerial.read();
    byte Byte3 = mySerial.read();
    byte Byte4 = mySerial.read();
    byte Byte5 = mySerial.read();
    byte Byte6 = mySerial.read();
    byte Byte7 = mySerial.read();
    CompassAngle2 = (Byte2 - 0x30) * 100 + (Byte3 - 0x30) * 10 + (Byte4 - 0x30);//算出角度
    //Serial.println(CompassAngle2);
    if (CompassAngle2 <= 0 || CompassAngle2 >= 360)goto loop;
  }

    if (abs(CompassAngle2 - CompassAngle1) >= 5)goto loop;
    else CompassAngle0 = 0.5*(CompassAngle1 + CompassAngle2);
    Serial.println(CompassAngle0);


    return CompassAngle0;


  }

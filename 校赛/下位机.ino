/*
* 下位机 1.5
* 这个版本完整封装和整理了各个函数，并添加注释，已经过测试@0408
* 本次增加了直角转弯函数Rt***@0410
* 本次实试验性加入了巡线部分，已通过测试@0412
* 出现笔记本磁场干扰电子指南针情况@0412
* 放弃使用指南针，使用巡线传感器辅助转弯，未经过验证@0419
* 超声波传感器封装函数完成，未经过验证@0501
* by czl & robin
* 2019/05/01
*/

//引用的库函数
#include <SoftwareSerial.h>//软串口通信库函数（用于指南针）
SoftwareSerial mySerial(8, 9); //指南针模块的RX接8脚 TX接9脚

//全局函数声明--标星号的函数括号里为持续的时间，单位毫秒--
void xunxian(int);//巡线前进，括号里的变量为前进的格子数，函数内已清除使能端
void xstraight();//巡线时向前，数值监修中
void xleft(int);//巡线时调节向左,括号里的值代表调节的程度
void xright(int);//巡线时调节向右,括号里的值代表调节的程度
void forward(int);//*前进
void backward(int);//*后退
void left(int);//*向左，函数内已清除使能端
void right(int);//*向右，函数内已清除使能端
void Rtleft();//直角右转，函数内已清除使能端
void Rtright();//直角左转，函数内已清除使能端
void stoping(int);//*刹车，函数内已清除使能端
void clearing();//关闭使能端口
int delta(int, int);//检测转弯时角度是否达标
int zhinan();//用指南针测量目前方向角度
int shengbo();//用超声波传感器测量距离障碍物的距离

//arduino引脚声明
byte rightwheel0 = 5;
byte rightwheel1 = 7;
byte leftwheel0 = 4;
byte leftwheel1 = 2;
byte righten = 6;
byte leften = 3;//使能端
byte pingPin = 11; //Trig接11号引脚
byte echoPin = 12; // Echo接12号引脚
byte xun1=A1;
byte xun2=A2;
byte xun3=A3;
byte xun4=A4;//四个巡线传感器，车头远离人方向从左向右
byte zhuan1=A5;
byte zhuan2=A6;//两个位于车侧边的传感器用来确定转向90度

//全局变量声明
int val1, val2;//记录转弯前后角度值
int CompassAngle0, CompassAngle1, CompassAngle2;//记录指南针的测量值
int x1,x2,x3,x4,z1,z2;//记录6个巡线传感器的值
int flag0=0;//记录连续多少次遇到白线，用于数格子
int flag1=0,flag4=0;//记录数到第几个格子
bool flag2=0;//0指目前可以进行白线计数，1指不可以

//主函数开始
//-------------------------------------------
//-------------------------------------------
void setup() {
  //接口定义
  for (byte i = 2; i <8; i++)pinMode(i, OUTPUT);//定义2-7接口为输出
  // for (byte i = 10; i <14; i++)pinMode(i, INPUT);//定义10-13接口为输入
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);//超声波传感器的接口
  
  //串口定义
  Serial.begin(9600);//与电脑硬串口波特率9600
  mySerial.begin(9600);//指南针软串口波特率9600


  //初始化
  //for (byte i = 10; i <14; i++)digitalWrite(i, HIGH);//初始化巡线传感器
  clearing();//初始化马达
  delay(5000);//随便写的停顿
}

void loop() {
  Rtright();
  stoping(10000);
}
//------------------------------------------
//------------------------------------------

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

void Rtleft() {
  digitalWrite(leftwheel0, HIGH);
  digitalWrite(leftwheel1, LOW);
  analogWrite(leften, 255);
  //左边轮子反转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转
  delay(500);//延迟以免刚刚开始转动时遇到直行方向的白线
  
  z1=digitalRead(zhuan1);
  z2=digitalRead(zhuan2);
  while(!(z1==1&&z2==1)){//当没有遇到白线时，继续左转
    z1=digitalRead(zhuan1);
    z2=digitalRead(zhuan2);
    delay(5);
  }
  stoping(0);
}

void Rtright() {
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子正转

  digitalWrite(rightwheel0, HIGH);
  digitalWrite(rightwheel1, LOW);
  analogWrite(righten, 255);
  //右边轮子反转
  delay(500);//延迟以免刚刚开始转动时遇到直行方向的白线
  
  z1=digitalRead(zhuan1);
  z2=digitalRead(zhuan2);
  while(!(z1==1&&z2==1)){//当没有遇到白线时，继续左转
    z1=digitalRead(zhuan1);
    z2=digitalRead(zhuan2);
    delay(5);
  }
  stoping(0);
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
  clearing();
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
  clearing();
}

void stoping(int microseconds) {
  digitalWrite(leftwheel0, HIGH);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);

  digitalWrite(rightwheel0, HIGH);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  
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

int zhinan(){
  loop:mySerial.write(byte(0x31));//发送指令
  delay(200);
  if (mySerial.available()){//共返回8个字节
    byte Byte0 = mySerial.read();//开始读取缓冲区里的字节
    byte Byte1 = mySerial.read();//byte型变量为字节型
    byte Byte2 = mySerial.read();
    byte Byte3 = mySerial.read();
    byte Byte4 = mySerial.read();
    byte Byte5 = mySerial.read();
    byte Byte6 = mySerial.read();
    byte Byte7 = mySerial.read();
    CompassAngle1 = (Byte2 - 0x30) * 100 + (Byte3 - 0x30) * 10 + (Byte4 - 0x30);//算出角度
  }
  if (CompassAngle1<0 || CompassAngle1>360)goto loop;//如果收到异常值，重新测量

  mySerial.write(byte(0x31));//发送指令，第二次测量角度
  delay(200);
  if (mySerial.available()){//共返回8个字节
    byte Byte0 = mySerial.read();//开始读取缓冲区里的字节
    byte Byte1 = mySerial.read();//byte型变量为字节型
    byte Byte2 = mySerial.read();
    byte Byte3 = mySerial.read();
    byte Byte4 = mySerial.read();
    byte Byte5 = mySerial.read();
    byte Byte6 = mySerial.read();
    byte Byte7 = mySerial.read();
    CompassAngle2 = (Byte2 - 0x30) * 100 + (Byte3 - 0x30) * 10 + (Byte4 - 0x30);//算出角度
  }
  if (CompassAngle2 < 0 || CompassAngle2 > 360)goto loop;//如果收到异常值，重新测量
    
  if (abs(CompassAngle2 - CompassAngle1) >= 5)goto loop;//如果两次测出的值相差较大，重新测量
    else CompassAngle0 = 0.5*(CompassAngle1 + CompassAngle2);//求出最终值
    
  return CompassAngle0;
}

void xunxian(int gezi){
  while(flag1<gezi){
  x1= digitalRead(xun1);
  x2= digitalRead(xun2);
  x3= digitalRead(xun3);
  x4= digitalRead(xun4);//读取红外巡迹传感器的值
  float sum=0.35*x1+0.15*x2-0.15*x3-0.35*x4;//计算最终值,正偏右，负偏左
  Serial.print("巡迹数据：");
  Serial.print(x1);
  Serial.print(" ");
  Serial.print(x2);
  Serial.print(" ");
  Serial.print(x3);
  Serial.print(" ");
  Serial.println(x4);
  Serial.println(sum);//输出，用于debug
  if(sum==-0.5)xright(2);
  else if(sum==-0.35)xright(3);
  else if(sum==-0.15)xright(1);
  else if(sum==0.5)xleft(2);
  else if(sum==0.35)xleft(3);
  else if(sum==0.15)xleft(1);
  else xstraight();//对不同的情况进行左右调整
  if(x1==0&&x2==0)flag4++;
  else flag4=0;
  if(flag4>5)flag2=0;
  if(x1+x2+x3+x4>=3&&flag2==0)flag0++;//如果发现遇到白线，则flag0+1
  else flag0=0;
  if(flag0==2){//多次发现白线，可以确定是真的白线
    flag1++;
    flag0=0;
    flag2=0;
  }
  }
  flag0=0;
  flag1=0;
  flag2=1;//巡线结束，清空标志值
  stoping(0);
}

void xleft(int flag3){
  Serial.println("向左调整");
  if(flag3==1){
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 100);
  //左边轮子正转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转
  }
  
  if(flag3==2){
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 50);
  //左边轮子正转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转
  }
  
  if(flag3==3){
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 0);
  //左边轮子正转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转
  }

  delay(5);
}

void xright(int flag3){
  Serial.println("向右调整");
  if(flag3==1){
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子正转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 100);
  //右边轮子正转
  }
  
  if(flag3==2){
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子正转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 50);
  //右边轮子正转
  }
  
  if(flag3==3){
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子正转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 0);
  //右边轮子正转
  }
  
  delay(5);
}

void xstraight(){
  Serial.println("向前");
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子正转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转

  delay(5);
}

int shengbo(){
   int duration, cm1，cm2,cm3,cm;//测到的距离和转化为厘米单位的距离值
   int cha1,cha2,cha3;//三个值的差
   
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm1 =duration / 29 / 2 ;
   Serial.print("第一次测量:");
   Serial.print(cm1);//以厘米为单位
   delay(10);
   
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm2 =duration / 29 / 2 ;
   Serial.print("第二次测量:");
   Serial.print(cm2);//以厘米为单位
   delay(10);//测量之间的等待时间为10ms
   
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm3 =duration / 29 / 2 ;
   Serial.print("第三次测量:");
   Serial.println(cm3);//以厘米为单位
   
   cha1=abs(cm1-cm2);
   cha2=abs(cm2-cm3);
   cha3=abs(cm1-cm3);
   if(cha2>cha1&&cha3>cha1)cm=1/2(cm1+cm2);
   if(cha1>cha2&&cha3>cha2)cm=1/2(cm2+cm3);
   if(cha1>cha3&&cha2>cha3)cm=1/2(cm1+cm3);
   
   Serial.print("实际距离：");
   Serial.println(cm);
   return cm;
}

/*
* 下位机 1.2
* 这个版本完整封装和整理了各个函数，并添加注释，已经过测试@0408
* 本次增加了直角转弯函数Rt***@0410
* 本次实试验性加入了巡线部分，但还未经过验证@0411
* by czl & robin
* 2019/04/11
*/

//引用的库函数
#include <SoftwareSerial.h>//软串口通信库函数（用于指南针）
SoftwareSerial mySerial(8, 9); //指南针模块的RX接8脚 TX接9脚

//全局函数声明--标星号的函数括号里为持续的时间，单位毫秒--
void xunxian(int);//巡线前进，括号里的变量为前进的格子数
void xstraight();//巡线时向前，数值监修中
void xleft(int);//巡线时调节向左,括号里的值代表调节的程度，数值监修中
void xright(int);//巡线时调节向右,括号里的值代表调节的程度，数值监修中
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

//arduino引脚声明
byte rightwheel0 = 5;
byte rightwheel1 = 7;
byte leftwheel0 = 4;
byte leftwheel1 = 2;
byte righten = 6;
byte leften = 3;//使能端
byte xun1=A1;
byte xun2=A2;
byte xun3=A3;
byte xun4=A4;//四个巡线传感器，车头远离人方向从左向右排序

//全局变量声明
int val1, val2;//记录转弯前后角度值
int CompassAngle0, CompassAngle1, CompassAngle2;//记录指南针的测量值
int x1,x2,x3,x4;//记录四个巡线传感器的值
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
  val1 = zhinan();//第一次测量角度值
  Serial.print("第一次测量得到：");
  Serial.println(val1);//输出测量得到的角度
  
  digitalWrite(leftwheel0, HIGH);
  digitalWrite(leftwheel1, LOW);
  analogWrite(leften, 255);
  //左边轮子反转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转

  delay(700);//延时多少毫秒
  clearing();//停车等待角度测量

  val2 = zhinan();//第二次测量角度值
  Serial.print("第二次测量得到：");
  Serial.println(val2);

  while (delta(val1, val2) <= 85) {//最大允许误差和修正值后期视实际情况修改,如果转弯角度不合理，则修正
    left(100);
    val2 = zhinan();//再次测量现在的角度
  }
  while (delta(val1, val2) > 95) {//最大允许误差和修正值后期视实际情况修改
    right(50);
    val2 = zhinan();//再次测量现在的角度
  }

  clearing();
}

void Rtright() {
  val1 = zhinan();//第一次测量角度值
  Serial.print("第一次测量得到：");
  Serial.println(val1);//输出测量得到的角度
  
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子正转

  digitalWrite(rightwheel0, HIGH);
  digitalWrite(rightwheel1, LOW);
  analogWrite(righten, 255);
  //右边轮子反转

  delay(1000);//延时多少毫秒
  clearing();//停车等待角度测量

  val2 = zhinan();//第二次测量角度值
  Serial.print("第二次测量得到：");
  Serial.println(val2);

  while (delta(val1, val2) <= 85) {//最大允许误差和修正值后期视实际情况修改,如果转弯角度不合理，则修正
    Serial.println("右调");
    Serial.println(delta(val1, val2));
    right(100);
    val2 = zhinan();//再次测量现在的角度
  }
  while (delta(val1, val2) > 95) {//最大允许误差和修正值后期视实际情况修改
    Serial.println("左调");
    Serial.println(delta(val1, val2));
    left(50);
    val2 = zhinan();//再次测量现在的角度
  }
  clearing();
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
  x1= digitalRead(A0);
  x2= digitalRead(xun2);
  x3= digitalRead(xun3);
  x4= digitalRead(xun4);//读取红外巡迹传感器的值
  float sum=0.35*x1+0.15*x2-0.15*x3-0.35*x4;//计算最终值,正偏左，负偏右
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

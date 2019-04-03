/*
 * 下位机主程序开工啦！
 * 下位机 1.0
 * by czl & robin
 * 2019/04/03
 */

//全局函数声明
void forward(int);//前进
void backward(int);//后退
void left(int);//向左
void right(int);//向右
void stoping(int);//刹车
void chearing();//关闭使能端口
//--控制车运动的函数括号里为持续的时间，单位毫秒--

//arduino引脚声明
byte rightwheel0=5;
byte rightwheel1=7;
byte leftwheel0=4;
byte leftwheel1=2;
byte righten=6;//使能端（右轮）
byte leften=3;//使能端（左轮）

//全局变量声明

//主函数开始
void setup(){
  for(byte i=2;i<=7;i++)
  pinMode(i,OUTPUT);//定义接口
}

void loop(){
  clearing();//关闭使能端口作为初始化
  delay(3000);
  right(1300);
  clearing();
  delay(5000);
}

//封装函数定义
void forward(int microseconds){
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

void backward(int microseconds){
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

void left(int microseconds){
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

void right(int microseconds){
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

void stoping(int microseconds){
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

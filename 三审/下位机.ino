//测试L298N与电机是否连接正常
//by czl
//2019/04/02

void straight();
byte rightwheel0=5;
byte rightwheel1=7;
byte leftwheel0=4;
byte leftwheel1=2;
byte righten=6;//使能端
byte leften=3;
void setup(){
  for(byte i=2;i<=7;i++)
  pinMode(i,OUTPUT);//定义接口
}
void loop(){
  straight ();
  delay (5000);
}
void straight(){
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
          //左边轮子正转
                    
  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
          //右边轮子正转
}

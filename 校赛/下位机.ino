/*
* 下位机 1.8
* 此版本完整封装和整理了各个函数，并添加注释，已经过测试@0408
* 本次增加了直角转弯函数Rt***@0410
* 本次实试验性加入了巡线部分，已通过测试@0412
* 出现笔记本磁场干扰电子指南针情况@0412
* 放弃使用指南针，使用巡线传感器辅助转弯，未经过验证@0419
* 超声波传感器封装函数完成，未经过验证@0501
* 增加串口通信模块，已通过验证@0502
* 上位机发现所有障碍物都没有的时候应该认为所有障碍物都存在@0504
* 调用VCAPI函数接收串口中文信息会出现乱码，未解决@0504
* 修正巡线程序当开始点就在白线上时，会出现少数格子的问题@0506
* 添加后退函数，可以用于小车后退到后面第一个交点中央，未经过检验@0506
* by czl & robin
* 2019/05/06
*/

//引用的库函数
#include <SoftwareSerial.h>//软串口通信库函数（用于指南针）
SoftwareSerial mySerial(8, 9); //指南针模块的RX接8脚 TX接9脚

                 //全局函数声明--标星号的函数括号里为持续的时间，单位毫秒--
void xunxian(int);//巡线前进，括号里的变量为前进的格子数，函数内已清除使能端，但未使用刹车
void bange();//走半格让车停在格点中央，函数内已清除使能端
void houtui();//后退让车停在格点中央，函数内已清除使能端
void banwan();//向左转半个弯，函数内已清除使能端，但未使用刹车
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
void uartReceive();//用于接收串口发来的字符串
void head(int, char);//根据车现有方向和目标方向调整车的方向
void M1();//由于M1命令比较繁琐，由该函数辅助完成

           //arduino引脚声明
byte rightwheel0 = 5;
byte rightwheel1 = 7;
byte leftwheel0 = 4;
byte leftwheel1 = 2;
byte righten = 6;
byte leften = 3;//使能端
byte pingPin = 11; //Trig接11号引脚
byte echoPin = 10; // Echo接10号引脚
byte xun1 = A1;
byte xun2 = A2;
byte xun3 = A3;
byte xun4 = A4;//四个巡线传感器，车头远离人方向从左向右
byte zhuan1 = A5;
byte zhuan2 = A6;//两个位于车侧边的传感器用来确定转向90度

         //全局变量声明
int val1, val2;//记录转弯前后角度值
int CompassAngle0, CompassAngle1, CompassAngle2;//记录指南针的测量值
int x1, x2, x3, x4, z1, z2;//记录6个巡线传感器的值
int flag0 = 0;//记录连续多少次遇到白线，用于数格子
int flag1 = 0, flag4 = 0;//记录数到第几个格子
bool flag2 = 0;//0指目前可以进行白线计数，1指不可以
String inString = "";//用于接收来自上位机的命令
String outString = "";//用于发送给上位机的指令
int m = 0, n = 8, h = 2;//m横坐标,n纵坐标,h头朝向，这些数据改变放在运动的封装函数里
char outchar;//这是一个字符与整形转化过程的中转站
String result = "";//这是暂时存放视觉识别结果的中转站
int step;//作为向前步数的中转站
String local[4]={"0508","0806","0603","0305"};//保存4个仓库地点
String A[3]={"0110","0210","0310"};//这是保存A货架放置物体的固定位置，比赛当天修改
int flag5=0;//这是保存A货架已经放到哪里了
           //byte head;//上位机给出的方向

           //主函数开始（以下位机开启为机器启动）
           //-------------------------------------------
           //-------------------------------------------
void setup() {
  //接口定义
  for (byte i = 2; i <8; i++)pinMode(i, OUTPUT);//定义2-7接口为输出
  pinMode(11, OUTPUT);
  pinMode(10, INPUT);
  // for (byte i = 10; i <14; i++)pinMode(i, INPUT);//定义10-13接口为输入
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);//超声波传感器的接口

              //串口定义
  Serial.begin(9600);//与电脑硬串口波特率9600
  mySerial.begin(9600);//指南针软串口波特率9600


             //初始化
             //for (byte i = 10; i <14; i++)digitalWrite(i, HIGH);//初始化巡线传感器
  clearing();//初始化马达
  delay(9000);//比赛开始后要停顿10s（少停一秒节约时间嘿嘿嘿）

        //开始运动
  Serial.println("启动小车！");
  xunxian(5);
  bange();//到达第一个货架前
      //拍照
  Serial.print("M5");//M5号命令是用来指示上位机拍照识别的
  uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
  inString = "";
  Serial.println("1号货架识别完毕");

  xunxian(3);
  bange();
  Rtleft();
  houtui();//后退到可能障碍物点的左边
  n++;//这里的坐标改变需要手动添加
  stoping(0);

  //下面测量旁边有没有障碍物，M6命令是用来说明有无障碍物
  if (shengbo()>0)Serial.print("M61N");//如果返回值满足某条件则告诉上位机没有障碍物
  if (shengbo()<0)Serial.print("M61Y");//如果返回值满足某条件则告诉上位机没有障碍物

  xunxian(3);
  bange();
  //拍照
  Serial.print("M5");//M5号命令是用来指示上位机拍照识别的
  uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
  inString = "";
  Serial.println("2号货架识别完毕");

  //下面测量旁边有没有障碍物
  if (shengbo()>0)Serial.print("M62N");//如果返回值满足某条件则告诉上位机没有障碍物
  if (shengbo()<0)Serial.print("M62Y");//如果返回值满足某条件则告诉上位机没有障碍物

  xunxian(3);
  bange();
  left(600);//转45度的样子，数值未监修

        //测量旁边有没有障碍物
  if (shengbo()>0)Serial.print("M63N");//如果返回值满足某条件则告诉上位机没有障碍物
  if (shengbo()<0)Serial.print("M63Y");//如果返回值满足某条件则告诉上位机没有障碍物

  banwan();
  xunxian(2);
  bange();
  //拍照
  Serial.print("M5");//M5号命令是用来指示上位机拍照识别的
  uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
  inString = "";//解析指令后一定要清空字符串
  Serial.println("3号货架识别完毕");

  //测量旁边有没有障碍物
  if (shengbo()>0)Serial.print("M64N");//如果返回值满足某条件则告诉上位机没有障碍物
  if (shengbo()<0)Serial.print("M64Y");//如果返回值满足某条件则告诉上位机没有障碍物

  xunxian(3);
  bange();
  left(600);//转45度的样子，数值未监修

        //测量旁边有没有障碍物
  if (shengbo()>0)Serial.print("M65N");//如果返回值满足某条件则告诉上位机没有障碍物
  if (shengbo()<0)Serial.print("M65Y");//如果返回值满足某条件则告诉上位机没有障碍物

  banwan();
  xunxian(2);
  bange();
  //拍照
  Serial.print("M5");//M5号命令是用来指示上位机拍照识别的
  uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
  inString = "";
  Serial.println("4号货架识别完毕");

  //测量旁边有没有障碍物
  if (shengbo()>0)Serial.print("M66N");//如果返回值满足某条件则告诉上位机没有障碍物
  if (shengbo()<0)Serial.print("M66Y");//如果返回值满足某条件则告诉上位机没有障碍物

  xunxian(3);
  Rtleft();
  //第二圈运动开始，抓取小方块
  xunxian(2);

  //开始询问上位机识别结果
  Serial.print("M71");//M7号命令是用来询问上位机识别结果的
  uartReceive();
  result=inString;//先将inString里的数据迁移进来
  inString = "";//清空
  if (result[0] == '1' || result[1] == '1' || result[2] == '1') {//如果检测得到1号货架上有小方块
    Rtleft();
    xstraight();
    delay(500);//向前走一点靠近货架以便抓取
    if (result[0] = '1') {
      Serial.print("M81");//M8命令是控制机械臂的
      delay(3000);//等待机械臂运动完毕，数值待监修
      houtui();//后退到原来那个点上
      M1();//开始制作M1指令
      outString.concat(A[flag5]);//这是第一个空柜子的摆放地点
      Serial.print(outString);//让上位机安排路径
      outString="";//发送指令以后清空
      uartReceive();//接收上位机指令
              //解析指令
      byte i = inString[2] - '0';//得出一共有几段路
      for (byte j = 0; j<i; j++) {//循环判断路径
        head(h, inString[2*j+3]);//转弯
        step=inString[2*j+4]-'0';
        xunxian(step);
      }
      inString="";
      head(h,'3');//把车头调向正对A仓库
      Serial.print("M84");//让机械臂放回物品
      delay(3000);
      flag5++;
    }
    if(result[1]=='1'){//1号台中间有小方块
      if(m!=5||n!=8){//如果此时小车不在1号货架前
        M1();//开始制作M1指令
        outString.concat(local[0]);
        Serial.print(outString);//让上位机安排路径
        outString="";//发送指令以后清空
        uartReceive();//接收上位机指令
                //解析指令
        byte i = inString[2] - '0';//得出一共有几段路
        for (byte j = 0; j<i; j++) {//循环判断路径
          head(h, inString[2*j+3]);//转弯
          step=inString[2*j+4]-'0';
          xunxian(step);
        }
        inString="";
        head(h,'1');//把车头调向正对1号仓库
        xstraight();
        delay(500);//向前走一点靠近货架以便抓取
      }
      Serial.print("M82");//让机械臂取物品
      delay(3000);
      M1();//开始制作M1指令
      outString.concat(A[flag5]);
      Serial.print(outString);//让上位机安排路径
      outString="";//发送指令以后清空
      uartReceive();//接收上位机指令
              //解析指令
      byte i = inString[2] - '0';//得出一共有几段路
      for (byte j = 0; j<i; j++) {//循环判断路径
        head(h, inString[2*j+3]);//转弯
        step=inString[2*j+4]-'0';
        xunxian(step);
      }
      inString="";
      head(h,'3');//把车头调向正对A仓库
      Serial.print("M84");//让机械臂放置物品
      delay(3000);
      flag5++;
    }
    if(result[2]='1'){
      if(m!=5||n!=8){//如果此时小车不在1号货架前
        M1();//开始制作M1指令
        outString.concat(local[0]);
        Serial.print(outString);//让上位机安排路径
        outString="";//发送指令以后清空
        uartReceive();//接收上位机指令
                //解析指令
        byte i = inString[2] - '0';//得出一共有几段路
        for (byte j = 0; j<i; j++) {//循环判断路径
          head(h, inString[2*j+3]);//转弯
          step=inString[2*j+4]-'0';
          xunxian(step);
        }
        inString="";
        head(h,'1');//把车头调向正对1号仓库
        xstraight();
        delay(500);//向前走一点靠近货架以便抓取
      }
      Serial.print("M82");//让机械臂取物品
      delay(3000);
      M1();//开始制作M1指令
      outString.concat(A[flag5]);
      Serial.print(outString);//让上位机安排路径
      outString="";//发送指令以后清空
      uartReceive();//接收上位机指令
              //解析指令
      byte i = inString[2] - '0';//得出一共有几段路
      for (byte j = 0; j<i; j++) {//循环判断路径
        head(h, inString[2*j+3]);//转弯
        step=inString[2*j+4]-'0';
        xunxian(step);
      }
      inString="";
      head(h,'3');//把车头调向正对A仓库
      Serial.print("M84");//让机械臂放置物品
      delay(3000);
      flag5++;
    }
    //下一步就是把这些针对A货架的内容封装一下
  }
}

void loop() {}
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

  z1 = digitalRead(zhuan1);
  z2 = digitalRead(zhuan2);
  Serial.print("侧边红外数据：");
  Serial.print(z1);
  Serial.print(" ");
  Serial.println(z2);
  while (!(z1 == 1 && z2 == 1)) {//当没有遇到白线时，继续左转
    z1 = digitalRead(zhuan1);
    z2 = digitalRead(zhuan2);
    Serial.print("侧边红外数据：");
    Serial.print(z1);
    Serial.print(" ");
    Serial.println(z2);
    delay(5);
  }
  stoping(0);

  if (h == 1)h = 4;//turn head
  else h--;
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

  z1 = digitalRead(zhuan1);
  z2 = digitalRead(zhuan2);
  Serial.print("侧边红外数据：");
  Serial.print(z1);
  Serial.print(" ");
  Serial.println(z2);
  while (!(z1 == 1 && z2 == 1)) {//当没有遇到白线时，继续左转
    z1 = digitalRead(zhuan1);
    z2 = digitalRead(zhuan2);
    Serial.print("侧边红外数据：");
    Serial.print(z1);
    Serial.print(" ");
    Serial.println(z2);
    delay(5);
  }
  stoping(0);

  if (h == 4)h = 1;//turn head
  else h++;
}

void banwan() {
  digitalWrite(leftwheel0, HIGH);
  digitalWrite(leftwheel1, LOW);
  analogWrite(leften, 255);
  //左边轮子反转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转
  //delay();//延迟以免刚刚开始转动时遇到直行方向的白线

  z1 = digitalRead(zhuan1);
  z2 = digitalRead(zhuan2);
  Serial.print("侧边红外数据：");
  Serial.print(z1);
  Serial.print(" ");
  Serial.println(z2);
  while (!(z1 == 1 && z2 == 1)) {//当没有遇到白线时，继续左转
    z1 = digitalRead(zhuan1);
    z2 = digitalRead(zhuan2);
    Serial.print("侧边红外数据：");
    Serial.print(z1);
    Serial.print(" ");
    Serial.println(z2);
    delay(5);
  }
  clearing();

  if (h == 1)h = 4;//turn head
  else h--;
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

void clearing() {
  analogWrite(leften, 0);
  analogWrite(righten, 0);
}


int delta(int val1, int val2) {
  if (abs(val1 - val2)>180)
    return abs(abs(val1 - val2) - 360);
  else
    return abs(val1 - val2);
}

int zhinan() {
loop:mySerial.write(byte(0x31));//发送指令
  delay(200);
  if (mySerial.available()) {//共返回8个字节
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
  if (mySerial.available()) {//共返回8个字节
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

void xunxian(int gezi) {
  x1 = digitalRead(xun1);
  x2 = digitalRead(xun2);
  x3 = digitalRead(xun3);
  x4 = digitalRead(xun4);//读取红外巡迹传感器的值
  while (x1 == 1 && x2 == 1 && x3 == 1 && x4 == 1) {//刚刚开始就在白线上，那么这次白线就不能算数
    x1 = digitalRead(xun1);
    x2 = digitalRead(xun2);
    x3 = digitalRead(xun3);
    x4 = digitalRead(xun4);//读取红外巡迹传感器的值
    delay(5);//5ms后再次测量
  }
  while (flag1<gezi) {
    x1 = digitalRead(xun1);
    x2 = digitalRead(xun2);
    x3 = digitalRead(xun3);
    x4 = digitalRead(xun4);//读取红外巡迹传感器的值
    float sum = 0.35*x1 + 0.15*x2 - 0.15*x3 - 0.35*x4;//计算最终值,正偏右，负偏左
    Serial.print("巡迹数据：");
    Serial.print(x1);
    Serial.print(" ");
    Serial.print(x2);
    Serial.print(" ");
    Serial.print(x3);
    Serial.print(" ");
    Serial.println(x4);
    Serial.println(sum);//输出，用于debug
    if (sum == -0.5)xright(2);
    else if (sum == -0.35)xright(3);
    else if (sum == -0.15)xright(1);
    else if (sum == 0.5)xleft(2);
    else if (sum == 0.35)xleft(3);
    else if (sum == 0.15)xleft(1);
    else xstraight();//对不同的情况进行左右调整
    if (x1 == 0 && x2 == 0)flag4++;
    else flag4 = 0;
    if (flag4>5)flag2 = 0;
    if (x1 + x2 + x3 + x4 >= 3 && flag2 == 0)flag0++;//如果发现遇到白线，则flag0+1
    else flag0 = 0;
    if (flag0 == 2) {//多次发现白线，可以确定是真的白线
      flag1++;
      flag0 = 0;
      flag2 = 0;
    }
  }
  flag0 = 0;
  flag1 = 0;
  flag2 = 1;//巡线结束，清空标志值
  clearing();

  if (h == 1)n--;
  else if (h == 2)m++;
  else if (h == 3)n++;
  else if (h == 4)m--;
}

void xleft(int flag3) {
  Serial.println("向左调整");
  if (flag3 == 1) {
    digitalWrite(leftwheel0, LOW);
    digitalWrite(leftwheel1, HIGH);
    analogWrite(leften, 100);
    //左边轮子正转

    digitalWrite(rightwheel0, LOW);
    digitalWrite(rightwheel1, HIGH);
    analogWrite(righten, 255);
    //右边轮子正转
  }

  if (flag3 == 2) {
    digitalWrite(leftwheel0, LOW);
    digitalWrite(leftwheel1, HIGH);
    analogWrite(leften, 50);
    //左边轮子正转

    digitalWrite(rightwheel0, LOW);
    digitalWrite(rightwheel1, HIGH);
    analogWrite(righten, 255);
    //右边轮子正转
  }

  if (flag3 == 3) {
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

void xright(int flag3) {
  Serial.println("向右调整");
  if (flag3 == 1) {
    digitalWrite(leftwheel0, LOW);
    digitalWrite(leftwheel1, HIGH);
    analogWrite(leften, 255);
    //左边轮子正转

    digitalWrite(rightwheel0, LOW);
    digitalWrite(rightwheel1, HIGH);
    analogWrite(righten, 100);
    //右边轮子正转
  }

  if (flag3 == 2) {
    digitalWrite(leftwheel0, LOW);
    digitalWrite(leftwheel1, HIGH);
    analogWrite(leften, 255);
    //左边轮子正转

    digitalWrite(rightwheel0, LOW);
    digitalWrite(rightwheel1, HIGH);
    analogWrite(righten, 50);
    //右边轮子正转
  }

  if (flag3 == 3) {
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

void xstraight() {
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

void bange() {
  digitalWrite(leftwheel0, LOW);
  digitalWrite(leftwheel1, HIGH);
  analogWrite(leften, 255);
  //左边轮子正转

  digitalWrite(rightwheel0, LOW);
  digitalWrite(rightwheel1, HIGH);
  analogWrite(righten, 255);
  //右边轮子正转

  z1 = digitalRead(zhuan1);
  z2 = digitalRead(zhuan2);
  Serial.print("侧边红外数据：");
  Serial.print(z1);
  Serial.print(" ");
  Serial.println(z2);
  while (!(z1 == 1 && z2 == 1)) {//当没有遇到白线时，继续向前
    z1 = digitalRead(zhuan1);
    z2 = digitalRead(zhuan2);
    Serial.print("侧边红外数据：");
    Serial.print(z1);
    Serial.print(" ");
    Serial.println(z2);
    delay(5);
  }
}

  void houtui(){
    digitalWrite(leftwheel0, HIGH);
    digitalWrite(leftwheel1, LOW);
    analogWrite(leften, 255);
    //左边轮子反转

    digitalWrite(rightwheel0, HIGH);
    digitalWrite(rightwheel1, LOW);
    analogWrite(righten, 255);
    //右边轮子反转

    z1 = digitalRead(zhuan1);
    z2 = digitalRead(zhuan2);
    Serial.print("侧边红外数据：");
    Serial.print(z1);
    Serial.print(" ");
    Serial.println(z2);
    while (z1 == 1 && z2 == 1) {//如果本来就在白线上，应该忽略
      z1 = digitalRead(zhuan1);
      z2 = digitalRead(zhuan2);
      delay(5);
    }
    while (!(z1 == 1 && z2 == 1)) {//当没有遇到白线时，继续向前
      z1 = digitalRead(zhuan1);
      z2 = digitalRead(zhuan2);
      Serial.print("侧边红外数据：");
      Serial.print(z1);
      Serial.print(" ");
      Serial.println(z2);
      delay(5);
    }
    stoping(0);
    Serial.println("到达格点中央");
  }

  int shengbo() {
    int duration, cm1, cm2, cm3, cm;//测到的距离和转化为厘米单位的距离值
    int cha1, cha2, cha3;//三个值的差

    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    cm1 = duration / 29 / 2;
    Serial.print("第一次测量:");
    Serial.print(cm1);//以厘米为单位
    delay(10);

    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    cm2 = duration / 29 / 2;
    Serial.print("第二次测量:");
    Serial.print(cm2);//以厘米为单位
    delay(10);//测量之间的等待时间为10ms

    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    cm3 = duration / 29 / 2;
    Serial.print("第三次测量:");
    Serial.println(cm3);//以厘米为单位

    cha1 = abs(cm1 - cm2);
    cha2 = abs(cm2 - cm3);
    cha3 = abs(cm1 - cm3);
    if (cha2>cha1&&cha3>cha1)cm = (cm1 + cm2) / 2;
    if (cha1>cha2&&cha3>cha2)cm = (cm2 + cm3) / 2;
    if (cha1>cha3&&cha2>cha3)cm = (cm1 + cm3) / 2;

    Serial.print("实际距离：");
    Serial.println(cm);
    return cm;
  }

  void uartReceive() {
    bool endflag = 1;
    while (endflag == 1) {
      while (Serial.available()>0) {   //如果串口有数据
        char inChar = Serial.read(); //读取串口字符
                       //inString += inChar;
        inString.concat(inChar);     //连接接收到的字符组
        delayMicroseconds(100);      //为了防止数据丢失,在此设置短暂延时100us
        Serial.flush();             //清空串口接收缓存
        if (inString.length() > 200) {
          inString = "";
        }
        endflag = 0;
      }
    }
  }

  void head(int h, char direction) {
    byte fangxiang = direction - '0';//fangxiang是指目标方向
    if (fangxiang == h)return;
    if (h == 1) {
      if (fangxiang == 2)Rtright();
      else if (fangxiang == 3) {
        Rtright();
        Rtright();//掉头
      }
      else Rtleft();
    }
    if (h == 2) {
      if (fangxiang == 1)Rtleft();
      else if (fangxiang == 3)Rtright();
      else {
        Rtright();
        Rtright();//掉头
      }
    }
    if (h == 3) {
      if (fangxiang == 1) {
        Rtright();
        Rtright();
      }
      if (fangxiang == 2)Rtleft();
      else Rtright();
    }
    if (h == 4) {
      if (fangxiang == 1)Rtright();
      if (fangxiang == 3)Rtleft();
      else {
        Rtright();
        Rtright();
      }
    }
  }

void M1(){
  outString = "M1";
  outchar = '0' + m / 10;//先得到m的十位数
  outString.concat(outchar);
  outchar = '0' + m % 10;//再得到个位数
  outString.concat(outchar);
  outchar = '0' + n / 10;//先得到n的十位数
  outString.concat(outchar);
  outchar = '0' + n % 10;//再得到个位数
  outString.concat(outchar);
}

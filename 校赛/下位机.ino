/*
* 下位机 2.1 beta
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
* 解决启动区物理识别误差问题@0507
* 启动区红色识别会影响巡线，把这个格点作为障碍物点@0507
* 删除了电子指南针所有的遗留程序@0508
* 总体程序完成，正式进入debug阶段@0508
* 使用delay转弯，不仅偏心而且不准,打算后退也巡线@0508
* 用于机械臂的delay要修改@0510
* 使用了起始符和终止符，串口稳定性增加@0510
* by czl & robin
* 2019/05/10
*/

//引用的库函数
//暂无

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
int shengbo();//用超声波传感器测量距离障碍物的距离
int shengbo1();//上方
int shengbo2();//下方
void uartReceive();//用于接收串口发来的字符串
void head(int, char);//根据车现有方向和目标方向调整车的方向
void M1();//由于M1命令比较繁琐，由该函数辅助完成
void Ahuojia(char,char,int,char);//第一个是m坐标，第二个是n坐标，第三个是E仓库方向,第四个是抓中间还是旁边
//1,2,3分别表示左中右；4,5分别表示下上
void Bhuojia(char,char,int,char);//同上
void Chuojia(char,char,int,char);
void Dhuojia(char,char,int,char);
void tance(int);//超声波探测各货架上有没有货物，int是指在哪个货架上
void tiaozheng(int,int,char);//E货架的坐标点，和目标运动方向（平行于货架）
void xunxians();//接近E货架

           //arduino引脚声明
byte rightwheel0 = 5;
byte rightwheel1 = 7;
byte leftwheel0 = 4;
byte leftwheel1 = 2;
byte righten = 6;
byte leften = 3;//使能端
byte pingPin = 11; //Trig接11号引脚
byte echoPin = 10; // Echo接10号引脚
byte pingPin1 = 9;
byte echoPin1 = 8; //这是检测上方格子的超声波
byte pingPin2 = 12;
byte echoPin2 = 13; //这是检测下方格子的超声波
byte xun1 = A1;
byte xun2 = A2;
byte xun3 = A3;
byte xun4 = A4;//四个巡线传感器，车头远离人方向从左向右
byte zhuan1 = A5;
byte zhuan2 = A0;//两个位于车侧边的传感器用来确定转向90度

         //全局变量声明
int x1, x2, x3, x4, z1, z2;//记录6个巡线传感器的值
int flag0 = 0;//记录连续多少次遇到白线，用于数格子
int flag1 = 0, flag4 = 0;//记录数到第几个格子
bool flag2 = 0;//0指目前可以进行白线计数，1指不可以
String inString = "";//用于接收来自上位机的命令
String outString = "";//用于发送给上位机的指令
int m = 1, n = 8, h = 2;//m横坐标,n纵坐标,h头朝向，这些数据改变放在运动的封装函数里
char outchar;//这是一个字符与整形转化过程的中转站
String result = "";//这是暂时存放视觉识别结果的中转站
int step;//作为向前步数的中转站
String local[4]={"0508","0806","0603","0305"};//保存4个仓库地点
String A[3]={"01104","02104","03104"};//这是保存A货架放置物体的固定位置，最后一位是上下，比赛当天修改
int flag5=0;//这是保存A货架已经放到哪里了
int things[3][12];//二维数组保存货架有无货物的信息，顺序是货架从右到左，从上到下
int flag6=0;//这是保存超声波检测到货架的第几个了，每个货架结束以后要归零
int flag7=0;//这是保存后来放置物体时扫描数组里哪里是空着的
int flag10=0;//这是保存xunxians()函数中变量的

           //主函数开始（以下位机开启为机器启动）
           //-------------------------------------------
           //-------------------------------------------
void setup() {
  //接口定义
  for (int i = 2; i <8; i++)pinMode(i, OUTPUT);//定义2-7接口为输出
  pinMode(11, OUTPUT);
  pinMode(10, INPUT);
  pinMode(9, OUTPUT);
  pinMode(8, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  // for (byte i = 10; i <14; i++)pinMode(i, INPUT);//定义10-13接口为输入
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);//超声波传感器的接口

              //串口定义
  Serial.begin(9600);//与电脑硬串口波特率9600


             //初始化
  clearing();//初始化马达
  Serial.print("begin");
  delay(9000);//比赛开始后要停顿10s（少停一秒节约时间嘿嘿嘿）

        //开始运动
  Serial.println("M80");//让上位机保持拍照姿势
  xstraight();
  delay(1200);//因为传感器会把红色识别为白色，所以启动区不识别，直接向前
  xunxian(3);
  bange();//到达第一个货架前
      //拍照
  Serial.print("M51");//M5号命令是用来指示上位机拍照识别的
  uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
  inString = "";
  //Serial.println("1号货架识别完毕");

  xunxian(4);
  bange();
  Rtleft();

  //后退一格
  digitalWrite(leftwheel0, HIGH);
  digitalWrite(leftwheel1, LOW);
  analogWrite(leften, 255);
  //左边轮子反转
  digitalWrite(rightwheel0, HIGH);
  digitalWrite(rightwheel1, LOW);
  analogWrite(righten, 255);
  //右边轮子反转
  delay(500);//以免碰到刚才的白线
  z1 = digitalRead(zhuan1);
  z2 = digitalRead(zhuan2);
  /*Serial.print("侧边红外数据：");
  Serial.print(z1);
  Serial.print(" ");
  Serial.println(z2);*/
  while (!(z1 == 1 || z2 == 1)) {//当没有遇到白线时，继续后退
    z1 = digitalRead(zhuan1);
    z2 = digitalRead(zhuan2);
    /*Serial.print("侧边红外数据：");
    Serial.print(z1);
    Serial.print(" ");
    Serial.println(z2);*/
    delay(5);
  }
  n++;//这里的坐标改变需要手动添加
  stoping(0);

  //下面测量旁边有没有障碍物，M6命令是用来说明有无障碍物
  if (shengbo()>40)Serial.print("M61N");//如果返回值满足某条件则告诉上位机没有障碍物
  else Serial.print("M61Y");//如果返回值满足某条件则告诉上位机有障碍物

  xunxian(2);
  bange();
  //拍照
  Serial.print("M51");//M5号命令是用来指示上位机拍照识别的
  uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
  inString = "";
  xunxian(1);
  bange();
  //Serial.println("2号货架识别完毕");

  //下面测量旁边有没有障碍物
  if (shengbo()>40)Serial.print("M62N");//如果返回值满足某条件则告诉上位机没有障碍物
  else Serial.print("M62Y");//如果返回值满足某条件则告诉上位机没有障碍物

  xunxian(3);
  bange();
  left(500);//转45度的样子，数值未监修
  stoping(0);

        //测量旁边有没有障碍物
  if (shengbo()>40)Serial.print("M63N");//如果返回值满足某条件则告诉上位机没有障碍物
  else Serial.print("M63Y");//如果返回值满足某条件则告诉上位机没有障碍物

  banwan();
  xunxian(1);
  bange();
  //拍照
  Serial.print("M51");//M5号命令是用来指示上位机拍照识别的
  uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
  inString = "";//解析指令后一定要清空字符串
  xunxian(1);
  bange();
  //Serial.println("3号货架识别完毕");

  //测量旁边有没有障碍物
  if (shengbo()>40)Serial.print("M64N");//如果返回值满足某条件则告诉上位机没有障碍物
  else Serial.print("M64Y");//如果返回值满足某条件则告诉上位机没有障碍物

  xunxian(3);
  bange();
  left(500);//转45度的样子，数值未监修
  stoping(0);

        //测量旁边有没有障碍物
  if (shengbo()>40)Serial.print("M65N");//如果返回值满足某条件则告诉上位机没有障碍物
  else Serial.print("M65Y");//如果返回值满足某条件则告诉上位机没有障碍物

  banwan();
  xunxian(1);
  bange();
  //拍照
  Serial.print("M51");//M5号命令是用来指示上位机拍照识别的
  uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
  inString = "";
  xunxian(1);
  bange();
  //Serial.println("4号货架识别完毕");

  //测量旁边有没有障碍物
  if (shengbo()>40)Serial.print("M66N");//如果返回值满足某条件则告诉上位机没有障碍物
  else Serial.print("M66Y");//如果返回值满足某条件则告诉上位机没有障碍物

  xunxian(3);
  bange();
  Rtleft();
  //第二圈运动开始，抓取小方块
 xunxian(2);
 bange();

 //开始询问上位机识别结果
 Serial.print("M7");//M7号命令是用来询问上位机识别结果的
 uartReceive();
 result=inString;//先将inString里的数据迁移进来
 inString = "";//清空

 //到1号货架前面
 if (result[1] == '1' || result[2] == '1' || result[3] == '1') {//如果检测得到1号货架上有小方块
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   /*Serial.print("头：");
   Serial.println(h);*/
   if(result[1]=='1')Ahuojia(local[0][1],local[0][3],1,'1');
   if(result[2]=='1')Ahuojia(local[0][1],local[0][3],1,'2');
   if(result[3]=='1')Ahuojia(local[0][1],local[0][3],1,'3');//一号货架，右边
 }
 tiaozheng(5,8,'2');

 //到2号货架前面
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[4]=='1'||result[5]=='1'||result[6]=='1'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[4]=='1')Ahuojia(local[1][1],local[1][3],4,'1');
   if(result[5]=='1')Ahuojia(local[1][1],local[1][3],4,'2');
   if(result[6]=='1')Ahuojia(local[1][1],local[1][3],4,'3');
 }
 tiaozheng(8,6,'1');
 //到3号货架前
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 Serial.print("M7");//再次询问上位机识别结果的
 uartReceive();
 result=inString;//先将inString里的数据迁移进来
 inString = "";//清空
 if (result[7]=='1'||result[8]=='1'||result[9]=='1'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[7]=='1')Ahuojia(local[2][1],local[2][3],3,'1');
   if(result[8]=='1')Ahuojia(local[2][1],local[2][3],3,'2');
   if(result[9]=='1')Ahuojia(local[2][1],local[2][3],3,'3');
 }
   tiaozheng(6,3,'4');
 //到4号货架前
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[10]=='1'||result[11]=='1'||result[12]=='1'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[10]=='1')Ahuojia(local[3][1],local[3][3],2,'1');
   if(result[11]=='1')Ahuojia(local[3][1],local[3][3],2,'2');
   if(result[12]=='1')Ahuojia(local[3][1],local[3][3],2,'3');
 }
 tiaozheng(6,3,'3');
 flag5=0;//标志值归零

 //到B货架前面
 xunxian(5);
 bange();
 Rtleft();
 xunxian(7);
 bange();
 Rtleft();
 Serial.print("M9");//M9号命令是用来探测货架上有没有东西
 uartReceive();//接收上位机指令
 inString="";
 tance(0);//超声波探测
 flag6=0;
 //到C货架前面
 xunxian(3);
 bange();
 Rtleft();
 tance(1);
 flag6=0;
 //到D货架前面
 xunxian(3);
 bange();
 Rtleft();
 tance(2);//这个函数要修改
 flag6=0;//标志值归零

 m=1;n=7;h=3;//也不知道方向位置计算是否有错误，矫正一下
 //至此，要开始货物搬运了

 Rtleft();
 xunxian(2);
 bange();
 Rtright();
 xunxian(1);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 //到1号货架前面
 if (result[1] == '2' || result[2] == '2' || result[3] == '2'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[1]=='2')Bhuojia(local[0][1],local[0][3],1,'1');//一号货架，左边
   if(result[2]=='2')Bhuojia(local[0][1],local[0][3],1,'2');//一号货架，中间
   if(result[3]=='2')Bhuojia(local[0][1],local[0][3],1,'3');//一号货架，右边
 }
 tiaozheng(5,8,'2');
 //到2号货架前面
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[4]=='2'||result[5]=='2'||result[6]=='2'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[4]=='2')Bhuojia(local[1][1],local[1][3],4,'1');
   if(result[5]=='2')Bhuojia(local[1][1],local[1][3],4,'2');
   if(result[6]=='2')Bhuojia(local[1][1],local[1][3],4,'3');
 }
 tiaozheng(8,6,'1');
 //到3号货架前
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[7]=='2'||result[8]=='2'||result[9]=='2'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[7]=='2')Bhuojia(local[2][1],local[2][3],3,'1');
   if(result[8]=='2')Bhuojia(local[2][1],local[2][3],3,'2');
   if(result[9]=='2')Bhuojia(local[2][1],local[2][3],3,'3');
 }
 tiaozheng(6,3,'4');
 //到4号货架前
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[10]=='2'||result[11]=='2'||result[12]=='2'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[10]=='2')Bhuojia(local[3][1],local[3][3],2,'1');
   if(result[11]=='2')Bhuojia(local[3][1],local[3][3],2,'2');
   if(result[12]=='2')Bhuojia(local[3][1],local[3][3],2,'3');
 }
 tiaozheng(6,3,'3');
 flag7=0;

 //开始向C货架进军
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 //到1号货架前面
 if (result[1] == '3' || result[2] == '3' || result[3] == '3'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[1]=='3')Chuojia(local[0][1],local[0][3],1,'1');//一号货架，左边
   if(result[2]=='3')Chuojia(local[0][1],local[0][3],1,'2');//一号货架，中间
   if(result[3]=='3')Chuojia(local[0][1],local[0][3],1,'3');//一号货架，右边
 }
 tiaozheng(5,8,'2');
 //到2号货架前面
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[4]=='3'||result[5]=='3'||result[6]=='3'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[4]=='3')Chuojia(local[1][1],local[1][3],4,'1');
   if(result[5]=='3')Chuojia(local[1][1],local[1][3],4,'2');
   if(result[6]=='3')Chuojia(local[1][1],local[1][3],4,'3');
 }
 tiaozheng(8,6,'1');
 //到3号货架前
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[7]=='3'||result[8]=='3'||result[9]=='3'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[7]=='3')Chuojia(local[2][1],local[2][3],3,'1');
   if(result[8]=='3')Chuojia(local[2][1],local[2][3],3,'2');
   if(result[9]=='3')Chuojia(local[2][1],local[2][3],3,'3');
 }
 tiaozheng(6,3,'4');
 //到4号货架前
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[10]=='3'||result[11]=='3'||result[12]=='3'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[10]=='3')Chuojia(local[3][1],local[3][3],2,'1');
   if(result[11]=='3')Chuojia(local[3][1],local[3][3],2,'2');
   if(result[12]=='3')Chuojia(local[3][1],local[3][3],2,'3');
 }
 tiaozheng(6,3,'3');
 flag7=0;

 //开始向D货架进军
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 //到1号货架前面
 if (result[1] == '4' || result[2] == '4' || result[3] == '4'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[1]=='4')Dhuojia(local[0][1],local[0][3],1,'1');//一号货架，左边
   if(result[2]=='4')Dhuojia(local[0][1],local[0][3],1,'2');//一号货架，中间
   if(result[3]=='4')Dhuojia(local[0][1],local[0][3],1,'3');//一号货架，右边
 }
 tiaozheng(5,8,'2');
 //到2号货架前面
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[4]=='4'||result[5]=='4'||result[6]=='4'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[4]=='4')Dhuojia(local[1][1],local[1][3],4,'1');
   if(result[5]=='4')Dhuojia(local[1][1],local[1][3],4,'2');
   if(result[6]=='4')Dhuojia(local[1][1],local[1][3],4,'3');
 }
 tiaozheng(8,6,'1');
 //到3号货架前
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[7]=='4'||result[8]=='4'||result[9]=='4'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[7]=='4')Dhuojia(local[2][1],local[2][3],3,'1');
   if(result[8]=='4')Dhuojia(local[2][1],local[2][3],3,'2');
   if(result[9]=='4')Dhuojia(local[2][1],local[2][3],3,'3');
 }
 tiaozheng(6,3,'4');
 //到4号货架前
 xunxian(3);
 bange();
 Rtleft();
 xunxian(2);
 bange();
 if (result[10]=='4'||result[11]=='4'||result[12]=='4'){
   Rtleft();
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
   if(result[10]=='4')Dhuojia(local[3][1],local[3][3],2,'1');
   if(result[11]=='4')Dhuojia(local[3][1],local[3][3],2,'2');
   if(result[12]=='4')Dhuojia(local[3][1],local[3][3],2,'3');
 }
 tiaozheng(6,3,'3');
 flag7=0;

 //程序结束
 Serial.print("M4");
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
 delay(1150);//延迟以免刚刚开始转动时遇到直行方向的白线
 backward(30);

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
 delay(1150);//延迟以免刚刚开始转动时遇到直行方向的白线
 backward(30);

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
 delay(500);

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
   /*Serial.print("巡迹数据：");
   Serial.print(x1);
   Serial.print(" ");
   Serial.print(x2);
   Serial.print(" ");
   Serial.print(x3);
   Serial.print(" ");
   Serial.println(x4);
   Serial.println(sum);//输出，用于debug*/
   if (sum == -0.5)xright(2);
   else if (sum == -0.35)xright(3);
   else if (sum == -0.15)xright(1);
   else if (sum == 0.5)xleft(2);
   else if (sum == 0.35)xleft(3);
   else if (sum == 0.15)xleft(1);
   else xstraight();//对不同的情况进行左右调整

   if (x1 + x2 + x3 + x4 >= 3)flag0++;//如果发现遇到白线，则flag0+1
   else flag0 = 0;
   if (flag0 == 2) {//多次发现白线，可以确定是真的白线
     //Serial.println("格子+1");
     flag1++;
   }
 }
 flag0 = 0;
 flag1 = 0;//巡线结束，清空标志值
 clearing();

 if (h == 1)n-=gezi;
 else if (h == 2)m+=gezi;
 else if (h == 3)n+=gezi;
 else if (h == 4)m-=gezi;
}

void xunxians(){
   flag10=0;
   while(flag10<75){//现在延时200ms
   x1 = digitalRead(xun1);
   x2 = digitalRead(xun2);
   x3 = digitalRead(xun3);
   x4 = digitalRead(xun4);//读取红外巡迹传感器的值
   float sum = 0.35*x1 + 0.15*x2 - 0.15*x3 - 0.35*x4;//计算最终值,正偏右，负偏左
   if (sum == -0.5)xright(2);
   else if (sum == -0.35)xright(3);
   else if (sum == -0.15)xright(1);
   else if (sum == 0.5)xleft(2);
   else if (sum == 0.35)xleft(3);
   else if (sum == 0.15)xleft(1);
   else xstraight();//对不同的情况进行左右调整
   flag10++;
   }

   clearing();
}

void xleft(int flag3) {
 //Serial.println("向左调整");
 if (flag3 == 1) {
   digitalWrite(leftwheel0, LOW);
   digitalWrite(leftwheel1, HIGH);
   analogWrite(leften, 50);
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
   digitalWrite(leftwheel0, HIGH);
   digitalWrite(leftwheel1, LOW);
   analogWrite(leften, 100);
   //左边轮子反转

   digitalWrite(rightwheel0, LOW);
   digitalWrite(rightwheel1, HIGH);
   analogWrite(righten, 255);
   //右边轮子正转
 }

 delay(5);
}

void xright(int flag3) {
 //Serial.println("向右调整");
 if (flag3 == 1) {
   digitalWrite(leftwheel0, LOW);
   digitalWrite(leftwheel1, HIGH);
   analogWrite(leften, 255);
   //左边轮子正转

   digitalWrite(rightwheel0, LOW);
   digitalWrite(rightwheel1, HIGH);
   analogWrite(righten, 50);
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

   digitalWrite(rightwheel0, HIGH);
   digitalWrite(rightwheel1, LOW);
   analogWrite(righten, 100);
   //右边轮子反转
 }

 delay(5);
}

void xstraight() {
 //Serial.println("向前");
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
 /*Serial.print("侧边红外数据：");
 Serial.print(z1);
 Serial.print(" ");
 Serial.println(z2);*/
 while (z1 == 0 && z2 == 0) {//当没有遇到白线时，继续向前
   z1 = digitalRead(zhuan1);
   z2 = digitalRead(zhuan2);
   /*Serial.print("侧边红外数据：");
   Serial.print(z1);
   Serial.print(" ");
   Serial.println(z2);*/
   delay(5);
 }
 //如果还是太前面，要考虑是否加上后退的delay
 stoping(10);
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

   //delay(200);
   z1 = digitalRead(zhuan1);
   z2 = digitalRead(zhuan2);
   /*Serial.print("侧边红外数据：");
   Serial.print(z1);
   Serial.print(" ");
   Serial.println(z2);*/
   while (z1 == 1 && z2 == 1) {//如果本来就在白线上，应该忽略
     z1 = digitalRead(zhuan1);
     z2 = digitalRead(zhuan2);
     delay(5);
   }
   while (!(z1 == 1 || z2 == 1)) {//当没有遇到白线时，继续向前
     z1 = digitalRead(zhuan1);
     z2 = digitalRead(zhuan2);
     /*Serial.print("侧边红外数据：");
     Serial.print(z1);
     Serial.print(" ");
     Serial.println(z2);*/
     delay(5);
   }
   stoping(0);
   //Serial.println("到达格点中央");
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
   /*Serial.print("第一次测量:");
   Serial.print(cm1);//以厘米为单位*/
   delay(10);

   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm2 = duration / 29 / 2;
   //Serial.print("第二次测量:");
   //Serial.print(cm2);//以厘米为单位
   delay(10);//测量之间的等待时间为10ms

   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm3 = duration / 29 / 2;
   //Serial.print("第三次测量:");
   //Serial.println(cm3);//以厘米为单位

   cha1 = abs(cm1 - cm2);
   cha2 = abs(cm2 - cm3);
   cha3 = abs(cm1 - cm3);
   if (cha2>=cha1&&cha3>=cha1)cm = (cm1 + cm2) / 2;
   else if (cha1>=cha2&&cha3>=cha2)cm = (cm2 + cm3) / 2;
   else cm = (cm1 + cm3) / 2;

   //Serial.print("实际距离：");
   //Serial.println(cm);
   return cm;
 }

int shengbo1() {
     int duration, cm1, cm2, cm3, cm;//测到的距离和转化为厘米单位的距离值
     int cha1, cha2, cha3;//三个值的差

     digitalWrite(pingPin1, LOW);
     delayMicroseconds(2);
     digitalWrite(pingPin1, HIGH);
     delayMicroseconds(10);
     digitalWrite(pingPin1, LOW);
     duration = pulseIn(echoPin1, HIGH);
     cm1 = duration / 29 / 2;
     //Serial.print("第一次测量:");
     //Serial.print(cm1);//以厘米为单位
     delay(10);

     digitalWrite(pingPin1, LOW);
     delayMicroseconds(2);
     digitalWrite(pingPin1, HIGH);
     delayMicroseconds(10);
     digitalWrite(pingPin1, LOW);
     duration = pulseIn(echoPin1, HIGH);
     cm2 = duration / 29 / 2;
     //Serial.print("第二次测量:");
     //Serial.print(cm2);//以厘米为单位
     delay(10);//测量之间的等待时间为10ms

     digitalWrite(pingPin1, LOW);
     delayMicroseconds(2);
     digitalWrite(pingPin1, HIGH);
     delayMicroseconds(10);
     digitalWrite(pingPin1, LOW);
     duration = pulseIn(echoPin1, HIGH);
     cm3 = duration / 29 / 2;
     //Serial.print("第三次测量:");
     //Serial.println(cm3);//以厘米为单位

     cha1 = abs(cm1 - cm2);
     cha2 = abs(cm2 - cm3);
     cha3 = abs(cm1 - cm3);
     if (cha2>=cha1&&cha3>=cha1)cm = (cm1 + cm2) / 2;
     else if (cha1>=cha2&&cha3>=cha2)cm = (cm2 + cm3) / 2;
     else cm = (cm1 + cm3) / 2;

     //Serial.print("实际距离：");
     //Serial.println(cm);
     return cm;
   }

int shengbo2() {
       int duration, cm1, cm2, cm3, cm;//测到的距离和转化为厘米单位的距离值
       int cha1, cha2, cha3;//三个值的差

       digitalWrite(pingPin2, LOW);
       delayMicroseconds(2);
       digitalWrite(pingPin2, HIGH);
       delayMicroseconds(10);
       digitalWrite(pingPin2, LOW);
       duration = pulseIn(echoPin2, HIGH);
       cm1 = duration / 29 / 2;
       //Serial.print("第一次测量:");
       //Serial.print(cm1);//以厘米为单位
       delay(10);

       digitalWrite(pingPin2, LOW);
       delayMicroseconds(2);
       digitalWrite(pingPin2, HIGH);
       delayMicroseconds(10);
       digitalWrite(pingPin2, LOW);
       duration = pulseIn(echoPin2, HIGH);
       cm2 = duration / 29 / 2;
       //Serial.print("第二次测量:");
       //Serial.print(cm2);//以厘米为单位
       delay(10);//测量之间的等待时间为10ms

       digitalWrite(pingPin2, LOW);
       delayMicroseconds(2);
       digitalWrite(pingPin2, HIGH);
       delayMicroseconds(10);
       digitalWrite(pingPin2, LOW);
       duration = pulseIn(echoPin2, HIGH);
       cm3 = duration / 29 / 2;
       //Serial.print("第三次测量:");
       //Serial.println(cm3);//以厘米为单位

       cha1 = abs(cm1 - cm2);
       cha2 = abs(cm2 - cm3);
       cha3 = abs(cm1 - cm3);
       if (cha2>=cha1&&cha3>=cha1)cm = (cm1 + cm2) / 2;
       else if (cha1>=cha2&&cha3>=cha2)cm = (cm2 + cm3) / 2;
       else cm = (cm1 + cm3) / 2;

       //Serial.print("实际距离：");
       //Serial.println(cm);
       return cm;
     }

void uartReceive() {
   inString="";
   bool endflag = 1;
   while (endflag == 1) {
     while (Serial.available()>0) {   //如果串口有数据
       char inChar = Serial.read(); //读取串口字符
                      //inString += inChar;
       inString.concat(inChar);     //连接接收到的字符组
       delay(1);      //为了防止数据丢失,在此设置短暂延时1ms
       if (inString.length() > 200) {
         inString = "";
       }
       if(inString[0]!='M'){
         inString="";
       }
       if(inChar=='!'){//发现结束符退出循环
         endflag=0;
         break;
       }
     }
   }
   Serial.flush();             //清空串口接收缓存
   //Serial.print("sign:");
   //Serial.println(inString);//这是debug用的
 }

void head(int h, char direction) {
   int fangxiang = direction - '0';//fangxiang是指目标方向
   Serial.print("目标方向：");
   Serial.println(fangxiang);
   if (fangxiang == h)return;
   else if (h == 1) {
     if (fangxiang == 2)Rtright();
     else if (fangxiang == 3) {
       Serial.println("1111");//M1441114111
       Rtright();
       Rtright();//掉头
     }
     else Rtleft();
   }
   else if (h == 2) {
     if (fangxiang == 1)Rtleft();
     else if (fangxiang == 3)Rtright();
     else {
       Rtright();
       Rtright();//掉头
     }
   }
   else if (h == 3) {
     if (fangxiang == 1) {
       Rtright();
       Rtright();
     }
     else if (fangxiang == 2)Rtleft();
     else Rtright();
   }
   else if (h == 4) {
     if (fangxiang == 1)Rtright();
     else if (fangxiang == 3)Rtleft();
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

void Ahuojia(char heng,char shu,int toward,char location){
 if((m+'0')!=heng||(n+'0')!=shu){//如果此时小车不在1号货架前
   M1();//开始制作M1指令
   if(toward==1)outString.concat(local[0]);
   else if(toward==4)outString.concat(local[1]);
   else if(toward==3)outString.concat(local[2]);
   else outString.concat(local[3]);
   Serial.print(outString);//让上位机安排路径
   outString="";//发送指令以后清空
   uartReceive();//接收上位机指令
           //解析指令
   int i = inString[2] - '0';//得出一共有几段路
   Serial.print("一共路的段数：");
   Serial.println(i);
   for (int j = 0; j<i; j++) {//循环判断路径
     head(h, inString[2*j+3]);//转弯
     Serial.println("转弯");
     step=inString[2*j+4]-'0';
     Serial.print("步数：");
     Serial.println(step);
     xunxian(step);
     Serial.println("xun");
     bange();
   }
   inString="";
   head(h,'0'+toward);//把车头调向正对仓库
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
 }
 outString="M8";
 outString.concat(location);
 Serial.print(outString);//让机械臂取物品
 outString="";
 uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
 inString = "";
 houtui();
 M1();//开始制作M1指令
 outString.concat(A[flag5]);
 Serial.print(outString);//让上位机安排路径
 outString="";//发送指令以后清空
 uartReceive();//接收上位机指令
         //解析指令
 int i = inString[2] - '0';//得出一共有几段路
   Serial.print("一共路的段数：");
   Serial.println(i);
   for (int j = 0; j<i; j++) {//循环判断路径
     head(h, inString[2*j+3]);//转弯
     Serial.println("转弯");
     step=inString[2*j+4]-'0';
     Serial.print("步数：");
     Serial.println(step);
     xunxian(step);
     Serial.println("xun");
     bange();
   }
 inString="";
 head(h,'3');//把车头调向正对A仓库
 backward(200);//后退一点方便机械臂放物体
 stoping(0);
 outString="M8";
 outString.concat(A[flag5][4]);
 Serial.print(outString);//让机械臂放置物品
 outString="";
 uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
 inString = "";
 forward(200);//回到原点
 stoping(0);
 flag5++;
}

void Bhuojia(char heng,char shu,int toward,char location){
 if((m+'0')!=heng||(n+'0')!=shu){//如果此时小车不在1号货架前
   M1();//开始制作M1指令
   if(toward==1)outString.concat(local[0]);
   else if(toward==4)outString.concat(local[1]);
   else if(toward==3)outString.concat(local[2]);
   else outString.concat(local[3]);
   Serial.print(outString);//让上位机安排路径
   outString="";//发送指令以后清空
   uartReceive();//接收上位机指令
           //解析指令
   int i = inString[2] - '0';//得出一共有几段路
   for (int j = 0; j<i; j++) {//循环判断路径
     head(h, inString[2*j+3]);//转弯
     step=inString[2*j+4]-'0';
     xunxian(step);
     bange();
   }
   inString="";
   head(h,'0'+toward);//把车头调向正对仓库
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
 }
 outString="M8";
 outString.concat(location);
 Serial.print(outString);//让机械臂取物品
 outString="";
 uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
 inString = "";
 houtui();
 M1();//开始制作M1指令
 while(things[0][flag7]!=0)flag7++;
 outString.concat("10");
 if(flag7==0||flag7==1)outString.concat("10");
 else{
   outString.concat("0");
   outchar='0'+(10-flag7/2);
   outString.concat(outchar);
 }
 Serial.print(outString);//让上位机安排路径
 outString="";//发送指令以后清空
 uartReceive();//接收上位机指令
         //解析指令
 int i = inString[2] - '0';//得出一共有几段路
 for (int j = 0; j<i; j++) {//循环判断路径
   head(h, inString[2*j+3]);//转弯
   step=inString[2*j+4]-'0';
   xunxian(step);
   bange();
 }
 inString="";
 head(h,'2');//把车头调向正对B仓库
 backward(200);
 stoping(0);
 outString="M8";
 if(flag7%2==1)outString.concat("4");//下方格子
 else outString.concat("5");//上方格子
 Serial.print(outString);//让机械臂放置物品
 outString="";
 uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
 inString = "";
 forward(200);
 stoping(0);
 flag7++;
}

void Chuojia(char heng,char shu,int toward,char location){
 if((m+'0')!=heng||(n+'0')!=shu){//如果此时小车不在1号货架前
   M1();//开始制作M1指令
   if(toward==1)outString.concat(local[0]);
   else if(toward==4)outString.concat(local[1]);
   else if(toward==3)outString.concat(local[2]);
   else outString.concat(local[3]);
   Serial.print(outString);//让上位机安排路径
   outString="";//发送指令以后清空
   uartReceive();//接收上位机指令
           //解析指令
   int i = inString[2] - '0';//得出一共有几段路
   for (int j = 0; j<i; j++) {//循环判断路径
     head(h, inString[2*j+3]);//转弯
     step=inString[2*j+4]-'0';
     xunxian(step);
     bange();
   }
   inString="";
   head(h,'0'+toward);//把车头调向正对仓库
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
 }
 outString="M8";
 outString.concat(location);
 Serial.print(outString);//让机械臂取物品
 outString="";
 houtui();
 M1();//开始制作M1指令
 while(things[1][flag7]!=0)flag7++;
 if(flag7==0||flag7==1)outString.concat("10");
 else{
   outString.concat("0");
   outchar='0'+(10-flag7/2);
   outString.concat(outchar);
 }
 outString.concat("01");
 Serial.print(outString);//让上位机安排路径
 outString="";//发送指令以后清空
 uartReceive();//接收上位机指令
         //解析指令
 int i = inString[2] - '0';//得出一共有几段路
 for (int j = 0; j<i; j++) {//循环判断路径
   head(h, inString[2*j+3]);//转弯
   step=inString[2*j+4]-'0';
   xunxian(step);
   bange();
 }
 inString="";
 head(h,'1');//把车头调向正对C仓库
 backward(200);
 stoping(0);
 outString="M8";
 if(flag7%2==1)outString.concat("4");//下方格子
 else outString.concat("5");//上方格子
 Serial.print(outString);//让机械臂放置物品
 outString="";
 uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
 inString = "";
 forward(200);
 stoping(0);
 flag7++;
}

void Dhuojia(char heng,char shu,int toward,char location){
 if((m+'0')!=heng||(n+'0')!=shu){//如果此时小车不在1号货架前
   M1();//开始制作M1指令
   if(toward==1)outString.concat(local[0]);
   else if(toward==4)outString.concat(local[1]);
   else if(toward==3)outString.concat(local[2]);
   else outString.concat(local[3]);
   Serial.print(outString);//让上位机安排路径
   outString="";//发送指令以后清空
   uartReceive();//接收上位机指令
           //解析指令
   int i = inString[2] - '0';//得出一共有几段路
   for (int j = 0; j<i; j++) {//循环判断路径
     head(h, inString[2*j+3]);//转弯
     step=inString[2*j+4]-'0';
     xunxian(step);
     bange();
   }
   inString="";
   head(h,'0'+toward);//把车头调向正对E仓库
   xunxians();//向前走一点靠近货架以便抓取
   clearing();
 }
 outString="M8";
 outString.concat(location);
 Serial.print(outString);//让机械臂取物品
 outString="";
 uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
 inString = "";
 houtui();
 M1();//开始制作M1指令
 while(things[2][flag7]!=0)flag7++;
 outString.concat("01");
 if(flag7==0||flag7==1)outString.concat("10");
 else{
   outString.concat("0");
   outchar='0'+(1+flag7/2);
   outString.concat(outchar);
 }
 Serial.print(outString);//让上位机安排路径
 outString="";//发送指令以后清空
 uartReceive();//接收上位机指令
         //解析指令
 int i = inString[2] - '0';//得出一共有几段路
 for (int j = 0; j<i; j++) {//循环判断路径
   head(h, inString[2*j+3]);//转弯
   step=inString[2*j+4]-'0';
   xunxian(step);
   bange();
 }
 inString="";
 head(h,'4');//把车头调向正对C仓库
 backward(200);
 stoping(0);
 outString="M8";
 if(flag7%2==1)outString.concat("4");//下方格子
 else outString.concat("5");//上方格子
 Serial.print(outString);//让机械臂放置物品
 outString="";
 uartReceive();//接收上位机拍照完毕的回复，开始下一个动作
 inString = "";
 forward(200);
 stoping(0);
 flag7++;
}

void tiaozheng(int ms,int ns,char towards){
 if(m!=ms||n!=ns){//如果此时小车不在1号货架前
   M1();//开始制作M1指令
   if(towards=='2')outString.concat(local[0]);
   else if(towards=='1')outString.concat(local[1]);
   else if(towards=='4')outString.concat(local[2]);
   else outString.concat(local[3]);
   Serial.print(outString);//让上位机安排路径
   outString="";//发送指令以后清空
   uartReceive();//接收上位机指令
           //解析指令
   int i = inString[2] - '0';//得出一共有几段路
   for (int j = 0; j<i; j++) {//循环判断路径
     head(h, inString[2*j+3]);//转弯
     step=inString[2*j+4]-'0';
     xunxian(step);
     bange();//继续前进到格点中央为止
   }
   inString="";
   head(h,towards);//把车头调向运动方向
 }
}

void tance(int number){
 for(int i=0;i<6;i++){
   if(shengbo1()<40)things[number][flag6]=1;//看起来有货物
   else things[number][flag6]=0;
   flag6++;
   if(shengbo2()<40)things[number][flag6]=1;//看起来有货物
   else things[number][flag6]=0;
   flag6++;
   xunxian(1);
   bange();
 }
}

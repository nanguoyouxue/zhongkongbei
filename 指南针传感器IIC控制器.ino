//GY-26指南针传感器IIC控制器 1.1 beta
//by lb
//2019/02/24/20:10

#include <Wire.h>//IIC函数库

void setup() {
  Wire.begin();                // IIC总线初始化
  Serial.begin(9600);          //设置波特率9600bps
}

byte Byte0=0;//读取第一个无意义的字节
byte Byte1=0;//读取第二个字节
byte Byte2=0;//读取第三个字节
byte Byte3=0;//读取第四个字节
byte Byte4=0;//读取第五个字节
byte Byte5=0;//读取第六个字节
byte Byte6=0;//读取第七个字节
byte Byte7=0;//读取第八个字节
float Byte8=0;//最后得出的

void loop() {
  // 开始向指南针发送信号
  Wire.beginTransmission(112); // 指南针的地址为112
                 //地址在发送时是 224 (0xE0)
                 //但是IIC的地址只取前7位（二进制），所以实际地址为112
  Wire.write(byte(0x00));      // 将寄存器转化为命令寄存器 (0x00)
  Wire.write(byte(0x31));      // 命令指南针进行角度测量 (0x50)
  Wire.endTransmission();      // 结束测量


  delay(70);                   //延时70ms，等待完成测量和接收数据（至少65ms）

                 //开始接收数据
  Wire.requestFrom(112, 8);    // 请求从指南针传感器中接收3个字节
  if (Wire.available()) { // 如果接收
    Byte0= Wire.read();  //读取第1个字节
    Byte1= Wire.read();  //读取第2个字节
    Byte2= Wire.read();  //读取第3个字节
    Byte3= Wire.read();  //读取第4个字节
    Byte4= Wire.read();  //读取第5个字节
    Byte5= Wire.read();  //读取第6个字节
    Byte6= Wire.read();  //读取第7个字节
    Byte7= Wire.read();  //读取第8个字节
  }
  Byte8=(Byte2-0x30)*100+(Byte3-0x30)*10+(Byte4-0x30)+(Byte6-0x30)*0.1//根据读取的数据算出偏移角
  Serial.print(Byte8,1);
  Serial.println("°");

  delay(250);//延时250ms
}

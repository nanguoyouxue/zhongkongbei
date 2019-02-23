//GY-26指南针传感器IIC控制器 1.0
//by czl
//2019/02/23/13:34

#include <Wire.h>//IIC函数库

void setup() {
  Wire.begin();                // IIC总线初始化
  Serial.begin(9600);          //设置波特率9600bps
}

int reading0 = 0;//读取第一个无意义的字节
int reading1=0;//读取第二个字节
int reading2=0;//读取第三个字节
float reading=0;//最后得出的

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
  Wire.requestFrom(112, 3);    // 请求从指南针传感器中接收3个字节
  if (3 <= Wire.available()) { // 如果三个字节接收完毕
    reading0= Wire.read();  //读取第1个字节
    reading1=Wire.read();  //读取第2个字节
    reading2=Wire.read(); //读取第3个字节
  }
  reading=(255*reading1+reading2)*0.1005025126;//根据读取的数据算出偏移角
  Serial.print(reading);
  Serial.println("°");

  delay(250);//延时250ms
}

//电子指南针传感器串口控制器 1.0
//by czl
//2019/02/23/18:13

#include <SoftwareSerial.h>//软串口通信库函数

SoftwareSerial mySerial(10, 11); //指南针模块的RX接11脚 TX接10脚

float Angle;//记录最后的角度

void setup()
{
    Serial.begin(9600);//与电脑硬串口波特率
    mySerial.begin(9600);//软串口波特率
}
void loop()
{
    mySerial.write(byte(0x31));//发送指令
    delay(200);
    while (mySerial.available())
    {
        byte Byte0 = mySerial.read();//开始读取缓冲区里的字节
        byte Byte1 = mySerial.read();//byte型变量为字节型
        byte Byte2 = mySerial.read();
        byte Byte3 = mySerial.read();
        byte Byte4 = mySerial.read();
        byte Byte5 = mySerial.read();
        byte Byte6 = mySerial.read();
        byte Byte7 = mySerial.read();
        Angle=(Byte2-0x30)*100+(Byte3-0x30)*10+(Byte4-0x30)+(Byte6-0x30)*0.1;
        Serial.print(Angle,1);//向硬串口发送数据
        Serial.println("°");
    }
    delay(300);
}

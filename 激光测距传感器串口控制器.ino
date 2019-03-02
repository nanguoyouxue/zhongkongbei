//激光传感器串口控制器 1.0
//by czl
//2019/03/02/20:55

/*最长距离为1.5m*/

#include <SoftwareSerial.h>//软串口通信库函数

SoftwareSerial mySerial(10, 11); //指南针模块的RX接11脚 TX接10脚

float distance;//记录距离

void setup()
{
    Serial.begin(9600);//与电脑硬串口波特率
    mySerial.begin(9600);//软串口波特率
    mySerial.write(0xA5);
    mySerial.write(0x50);
    mySerial.write(0xF5);//发送指令长距离检测模式
}
void loop()
{
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
        distance= (Byte4<<8) | Byte5;//算出实际距离
          Serial.println(distance,0);//向硬串口发送最后数据，不保留小数位
    }
    
    while(mySerial.available())
    byte Byte0 = mySerial.read();//清空缓冲区
    
    delay(300);//每次测定都延时30ms
}

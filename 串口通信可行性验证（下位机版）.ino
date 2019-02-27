//串口通信可行性验证（下位机版）1.1
//by czl
//2019/02/27/09:57
/*
 * 实现功能：当下位机发现指南针传感器指向北方（±2°）时，向上位机发送指令#00！（只发送一次），上位机收到指令后向机械臂发送指令，使之运动。
 * 1.改进了几个BUG
 * 2.arduino-->上位机通信验证完成
 */

#include <SoftwareSerial.h>//软串口通信库函数

SoftwareSerial mySerial(10, 11); //指南针模块的RX接11脚 TX接10脚

float CompassAngle;//记录角度

void setup()
{
    Serial.begin(9600);//与电脑硬串口波特率
    mySerial.begin(9600);//软串口波特率
}
void loop()
{
    mySerial.write(byte(0x31));//发送指令
    delay(200);
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
        CompassAngle=(Byte2-0x30)*100+(Byte3-0x30)*10+(Byte4-0x30)+(Byte6-0x30)*0.1;//算出角度
        if(CompassAngle<=2||CompassAngle>=358){
          Serial.print("#00!");//向硬串口发送指令
          delay(3000000);//如果发送了数据，则暂停3000s
        }
        
    }
    
    while(mySerial.available())
    byte Byte0 = mySerial.read();//清空缓冲区
    
    delay(30);//每次测定都延时30ms
}

//电子指南针传感器串口控制器1.1 beta
//by czl
//2019/02/23/19;26
/*
 * 1.添加了指南针校准程序
 * 2.每次处理完毕后都清理缓冲区
 */

#include <SoftwareSerial.h>//软串口通信库函数

SoftwareSerial mySerial(10, 11); //指南针模块的RX接11脚 TX接10脚

float CompassAngle;//记录角度

void setup()
{
    Serial.begin(9600);//与电脑硬串口波特率
    mySerial.begin(9600);//软串口波特率

    /*校准程序 beta*/
    /*
    Serial.println("3s后开始校准");
    delay(3000);
    jiaozhun:mySerial.write(0xC0);//开始校准
    delay(60000);//校准需要约一分钟，旋转一圈
    mySerial.write(0xC1);//校准结束
    
    if (mySerial.available())
    {
        byte Byte0 = mySerial.read();//开始读取缓冲区里的字节
        byte Byte1 = mySerial.read();
        byte Byte2 = mySerial.read();
        byte Byte3 = mySerial.read();
        byte Byte4 = mySerial.read();
        byte Byte5 = mySerial.read();
        byte Byte6 = mySerial.read();
        byte Byte7 = mySerial.read();
        CompassAngle=Byte6-0x30;
        if(CompassAngle==0){
        Serial.println("校准失败！10s后重新开始校准！");
        delay(10000);
        goto jiaozhun;//重复校准
        else {
        Serial.print("校准完成！校准等级：");
        Serial.print(Angle,0);//等级越高，校准程度越好
        return 0;//程序结束
        }
    }
    */
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
        CompassAngle=(Byte2-0x30)*100+(Byte3-0x30)*10+(Byte4-0x30)+(Byte6-0x30)*0.1;
        Serial.print(CompassAngle,1);//向硬串口发送数据
        Serial.println("°");
    }
    
    while(mySerial.available())
    Byte0 = mySerial.read();//清空缓冲区
    
    delay(300);
}

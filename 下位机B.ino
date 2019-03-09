//下位机B 1.0 beta
//by Robin
//2019/03/09/15:17
/*
 * 实现功能：下位机B根据下位机A发出的信号控制电机
 */

 #include <SoftwareSerial.h>//软串口通信库函数

 SoftwareSerial mySerial(0, 1); //RX/TX为0和1

void setup()
{
    mySerial.begin(9600);//软串口波特率
    for(i=2;i<=13;i++)
    pinMode(i，OUTPUT);
    byte Byte[13]; 
}

void loop()
{
    if (mySerial.available())//共返回13个字节
    {
         for(i=0;i<=12;i++)
         byte Byte[i] = mySerial.read();
     }    //读取字节
    
    if(Byte[9]==1) //1号电机收到使能信号
      { if(Byte[1]==1) //1号电机正转
         {digitalWrite(2, HIGH);
          digitalWrite(4, LOW);
          analogWrite(3, Byte[2]);}
        else  //1号电机反转
         {digitalWrite(2, LOW);
          digitalWrite(4, HIGH);
          analogWrite(3, Byte[2]);}
      }
    if(Byte[10]==1) // 2号电机收到使能信号
    { if(Byte[3]==1) //2号电机正转
         {digitalWrite(5, HIGH);
          digitalWrite(7, LOW);
          analogWrite(3, Byte[4]);}
       else  //2号电机反转
         {digitalWrite(5, LOW);
          digitalWrite(7, HIGH);
          analogWrite(6, Byte[4]);}
      }
    if(Byte[11]==1) // 3号电机收到使能信号
    { if(Byte[5]==1) //3号电机正转
         {digitalWrite(10, HIGH);
          digitalWrite(8, LOW);
          analogWrite(9, Byte[6]);}
        else  //3号电机反转
         {digitalWrite(10, LOW);
          digitalWrite(8, HIGH);
          analogWrite(9, Byte[6]);}
      }  
    if(Byte[12]==1) // 4号电机收到使能信号
    { if(Byte[7]==1) //4号电机正转
         {digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          analogWrite(11, Byte[8]);}
        else  //4号电机反转
         {digitalWrite(12, LOW);
          digitalWrite(13, HIGH);
          analogWrite(11, Byte[8]);}
      }  


       delay(1000); //延时1s
}

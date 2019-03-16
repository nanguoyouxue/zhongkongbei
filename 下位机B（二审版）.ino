//下位机B
//by Robin
//2019/03/14
void straight();
void back();
void left();
void right();
void braking();
void setup() {
    byte i;
    for(i=2;i<=13;i++)
    pinMode(i,OUTPUT);//定义接口
    
    analogWrite(3, 0);
    analogWrite(6, 0);
    analogWrite(9, 0);
    analogWrite(11, 0);

    straight();
    delay(3000);//前进3s

    right();//右转90
    
    left();//左转90

    breaking();//刹车

    back();
    delay(2000);//后退2s
    
    analogWrite(3, 0);
    analogWrite(6, 0);
    analogWrite(9, 0);
    analogWrite(11, 0);
    

}

void loop() {
  }







//以下五个函数
void straight()
{         digitalWrite(2, LOW);
          digitalWrite(4, HIGH);
          analogWrite(3, 255);
          //一号电机正转
          
          digitalWrite(5, HIGH);
          digitalWrite(7, LOW);
          analogWrite(6, 255);
          //二号电机正转
          
          digitalWrite(10, LOW);
          digitalWrite(8, HIGH);
          analogWrite(9, 255);
          //三号电机正转
          
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          analogWrite(11, 255);
          //四号电机正转
}
void back()
{          digitalWrite(2, HIGH);
          digitalWrite(4, LOW);
          analogWrite(3, 255);
          //一号电机反转
          
          digitalWrite(5, LOW);
          digitalWrite(7, HIGH);
          analogWrite(6, 255);
          //二号电机反转
          
          digitalWrite(10, HIGH);
          digitalWrite(8, LOW);
          analogWrite(9, 255);
          //三号电机反转
          
          digitalWrite(12, LOW);
          digitalWrite(13, HIGH);
          analogWrite(11, 255);
          //四号电机反转
  }
void left()
{//左转90
          digitalWrite(2, HIGH);
          digitalWrite(4, LOW);
          analogWrite(3, 255);
          //一号电机反转
          
          digitalWrite(5, LOW);
          digitalWrite(7, HIGH);
          analogWrite(6, 255);
          //二号电机反转
          
          digitalWrite(10, LOW);
          digitalWrite(8, HIGH);
          analogWrite(9, 255);
          //三号电机正转
          
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          analogWrite(11, 255);
          //四号电机正转
          delay(1800);

}
void right()
{//右转90
          digitalWrite(2, LOW);
          digitalWrite(4, HIGH);
          analogWrite(3, 255);
          //一号电机正转
          
          digitalWrite(5, HIGH);
          digitalWrite(7, LOW);
          analogWrite(6, 255);
          //二号电机正转
          
          digitalWrite(10, HIGH);
          digitalWrite(8, LOW);
          analogWrite(9, 255);
          //三号电机反转
          
          digitalWrite(12, LOW);
          digitalWrite(13, HIGH);
          analogWrite(11, 255);
          //四号电机反转
          delay(1800);
}
void braking()
{         digitalWrite(2, HIGH);
          digitalWrite(4, HIGH);
          analogWrite(3, 255);
          //一号电机正转
          
          digitalWrite(5, HIGH);
          digitalWrite(7, HIGH);
          analogWrite(6, 255);
          //二号电机正转
          
          digitalWrite(10, HIGH);
          digitalWrite(8, HIGH);
          analogWrite(9, 255);
          //三号电机反转
          
          digitalWrite(12, HIGH);
          digitalWrite(13, HIGH);
          analogWrite(11, 255);
          //四号电机反转
          delay(150);
  }

//路径规划器 1.0 beta
/*
实现功能：提供小车在货架与货架之间的路径解决方案
*/
//by czl
//2019/04/17

/*全局函数与变量声明✧(≖ ◡ ≖✿ */
int spot[12][12] = { {1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,1,1,1,1,0,0,0,1},{1,0,0,0,1,1,1,1,0,0,0,1},{1,0,0,0,1,1,1,1,0,0,0,1},{1,0,0,0,1,1,1,1,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1} };//0为可以到达的点，1为不可到达的点（包括货架与障碍物）
int barrier=1;//标记障碍物位置，先暂时是固定值
int m=0, n=8, x, y,a,b;//分别是开始点和结束点和过程坐标
int steps=0;//记录所走步数
int turns=0;//记录转弯数
int directions[20];//记录每一步的前进方向，初始值0，上1，右2，下3，左4，这个数组也是从1开始计数的
int number=1;//记录现在试探到第几步,从1开始计数
int best = 100;//记录最好的成绩
int bestroad[20];//保存最优路径
int beststep;//保存最优步数
int sum0=0;

void shitan();
void guihua();

String inString = "";

void  setup(){
  Serial.begin(9600);
}

void loop(){
  uartReceive();
  if(inString[0]=='#'){
    x=inString[1]-'0';
    y=inString[2]-'0';
    Serial.println(x+y);
    guihua();
  }
  inString="";
  
  delay(500);
}
//接收串口发来的字符串
void uartReceive(){        
    while (Serial.available()>0) {   //如果串口有数据
        char inChar = Serial.read(); //读取串口字符
        //inString += inChar;
        inString.concat(inChar);     //连接接收到的字符组
        delayMicroseconds(100);      //为了防止数据丢失,在此设置短暂延时100us
        Serial.flush();             //清空串口接收缓存
        if(inString.length() > 200){
            inString ="";
        }
    }
}
void guihua() {
  memset(directions, 0, sizeof(directions));

  switch (barrier) {//分情况给障碍物点赋值
  case 1:spot[9][9] = 1; break;
  case 2:spot[9][6] = 1; break;
  case 3:spot[9][2] = 1; break;
  case 4:spot[6][2] = 1; break;
  case 5:spot[2][2] = 1; break;
  case 6:spot[2][5] = 1; break;
  }
  
  a = m;
  b = n;//开始在出发点
  shitan();
  Serial.println("begin");
  for (int k = 1; k < beststep; k++) {
    if (bestroad[k] == 1)Serial.println("上");
    else if (bestroad[k] == 2)Serial.println("右");
    else if (bestroad[k] == 3)Serial.println("下");
    else if (bestroad[k] == 4)Serial.println("左");
  }
  
}


void shitan() {
  sum0++;
  Serial.println(sum0);
  if (a == x && b == y) {
    if (steps + turns < best) {
      for (int k = 1; k < number; k++) 
        bestroad[k] = directions[k];
      best = steps + turns;
      beststep = number;
      Serial.println("goon");
    }
  }

  if (abs(a - x) + abs(b - y) > 15 && steps == 5)return;
  if (abs(a - x) + abs(b - y) > 12 && steps == 8)return;
  if (abs(a - x) + abs(b - y) > 8 && steps == 12)return;
  if (abs(a - x) + abs(b - y) > 5 && steps == 15)return;
  if (steps > 18)return;

  if (directions[number-1] != 3&&spot[a][b-1]!=1) {//如果上一步没有向下走，而且上面没有障碍物，那么这步就可以向上走（否则就返回了）
    steps++;//步数+1
    if (directions[number - 1] != 1)turns++;//如果上一步没有向上走，那么就转了一次弯
    b -= 1;//走了一步
    directions[number] = 1;
    number += 1;
    shitan();
    number -= 1;
    b += 1;
    if (directions[number - 1] != 1)turns--;
    steps--;//如果搜到底了，递归回来
  }
  if (directions[number - 1] != 4 && spot[a+1][b] != 1) {//如果上一步没有向左走，而且右面没有障碍物，那么这步就可以向右走（否则就返回了）
    steps++;//步数+1
    if (directions[number - 1] != 2)turns++;//如果上一步没有向右走，那么就转了一次弯
    a+= 1;//走了一步
    directions[number] = 2;
    number += 1;
    shitan();
    number -= 1;
    a-= 1;
    if (directions[number - 1] != 2)turns--;
    steps--;//如果搜到底了，递归回来
  }
  if (directions[number - 1] != 1 && spot[a][b +1] != 1) {//如果上一步没有向上走，而且下面没有障碍物，那么这步就可以向下走（否则就返回了）
    steps++;//步数+1
    if (directions[number - 1] != 3)turns++;//如果上一步没有向下走，那么就转了一次弯
    b += 1;//走了一步
    directions[number] = 3;
    number += 1;
    shitan();
    number -= 1;
    b -= 1;
    if (directions[number - 1] != 3)turns--;
    steps--;//如果搜到底了，递归回来
  }
  if (directions[number - 1] != 2 && spot[a-1][b] != 1) {//如果上一步没有向右走，而且左面没有障碍物，那么这步就可以向左走（否则就返回了）
    steps++;//步数+1
    if (directions[number - 1] != 4)turns++;//如果上一步没有向左走，那么就转了一次弯
    a -= 1;//走了一步
    directions[number]=4;
    number += 1;
    shitan();
    number -= 1;
    a += 1;
    if (directions[number - 1] != 4)turns--;
    steps--;//如果搜到底了，递归回来
  }
}

//路径规划模拟器 1.4 beta
/*
实现功能：提供小车在货架与货架之间的路径解决方案
*/
//by czl & robin
//2019/04/24

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <winnt.h>
#include <iostream>
/*全局函数与变量声明✧(≖ ◡ ≖✿ */
int spot[12][12] = { {1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,1,1,1,1,0,0,0,1},{1,0,0,0,1,1,1,1,0,0,0,1},{1,0,0,0,1,1,1,1,0,0,0,1},{1,0,0,0,1,1,1,1,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,0,0,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1} };//0为可以到达的点，1为不可到达的点（包括货架与障碍物）
int barrier;//标记障碍物位置
int m, n, x, y,a,b;//分别是开始点和结束点和过程坐标
int steps=0;//记录所走步数
int turns=0;//记录转弯数
int direction[20];//记录每一步的前进方向，初始值0，上1，右2，下3，左4，这个数组也是从1开始计数的
int number=1;//记录现在试探到第几步,从1开始计数
int best = 100;//记录最好的成绩
int bestroad[20];//保存最优路径
int beststep;//保存最优步数
char line[20];//保存路径规划的串口指令
int count;//路径规划中计数
int i,j;//计数变量
int p=1;//路径规划相关

void shitan();


//-------(￣▽￣)／程序开始---------
int main() {
  memset(direction, 0, sizeof(direction));
  scanf_s("%d %d", &m, &n);//输入开始点
  scanf_s("%d %d", &x, &y);//输入结束点
  scanf_s("%d", &barrier);//输入障碍点

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

  for (int k = 1; k < beststep; k++) {
    if (bestroad[k] == 1)printf_s("上\n");
    else if (bestroad[k] == 2)printf_s("右\n");
    else if (bestroad[k] == 3)printf_s("下\n");
    else if (bestroad[k] == 4)printf_s("左 \n");
  }

  //将bestroad翻译成串口语言
  line[0]='#';
  line[1]='1';
  if(beststep==2) {line[3]='0'+bestroad[1];line[4]='1';}
  else{
  for(i=1,j=3;i<=beststep-1;i++)
    { if(i==1) count=1;
     else if(i>=2&&bestroad[i]==bestroad[i-1]) count++;
       else {line[j]='0'+bestroad[i-1];line[j+1]='0'+count;count=1;j=j+2;p++;}
   if(i==beststep-1) {line[j]='0'+bestroad[i];line[j+1]='0'+count;}
     }
  }
  line[2]='0'+p;
  printf_s("%s",line);

  return 0;
}


void shitan() {
  if (a == x && b == y) {
    if (steps + turns < best) {
      for (int k = 1; k < number; k++)
        bestroad[k] = direction[k];
      best = steps + turns;
      beststep = number;
    }
  }

  if (abs(a - x) + abs(b - y) > 15 && steps == 5)return;
  if (abs(a - x) + abs(b - y) > 12 && steps == 8)return;
  if (abs(a - x) + abs(b - y) > 8 && steps == 12)return;
  if (abs(a - x) + abs(b - y) > 5 && steps == 15)return;
  if (steps > 18)return;

  if (direction[number-1] != 3&&spot[a][b-1]!=1) {//如果上一步没有向下走，而且上面没有障碍物，那么这步就可以向上走（否则就返回了）
    steps++;//步数+1
    if (direction[number - 1] != 1)turns++;//如果上一步没有向上走，那么就转了一次弯
    b -= 1;//走了一步
    direction[number] = 1;
    number += 1;
    shitan();
    number -= 1;
    b += 1;
    if (direction[number - 1] != 1)turns--;
    steps--;//如果搜到底了，递归回来
  }
  if (direction[number - 1] != 4 && spot[a+1][b] != 1) {//如果上一步没有向左走，而且右面没有障碍物，那么这步就可以向右走（否则就返回了）
    steps++;//步数+1
    if (direction[number - 1] != 2)turns++;//如果上一步没有向右走，那么就转了一次弯
    a+= 1;//走了一步
    direction[number] = 2;
    number += 1;
    shitan();
    number -= 1;
    a-= 1;
    if (direction[number - 1] != 2)turns--;
    steps--;//如果搜到底了，递归回来
  }
  if (direction[number - 1] != 1 && spot[a][b +1] != 1) {//如果上一步没有向上走，而且下面没有障碍物，那么这步就可以向下走（否则就返回了）
    steps++;//步数+1
    if (direction[number - 1] != 3)turns++;//如果上一步没有向下走，那么就转了一次弯
    b += 1;//走了一步
    direction[number] = 3;
    number += 1;
    shitan();
    number -= 1;
    b -= 1;
    if (direction[number - 1] != 3)turns--;
    steps--;//如果搜到底了，递归回来
  }
  if (direction[number - 1] != 2 && spot[a-1][b] != 1) {//如果上一步没有向右走，而且左面没有障碍物，那么这步就可以向左走（否则就返回了）
    steps++;//步数+1
    if (direction[number - 1] != 4)turns++;//如果上一步没有向左走，那么就转了一次弯
    a -= 1;//走了一步
    direction[number]=4;
    number += 1;
    shitan();
    number -= 1;
    a += 1;
    if (direction[number - 1] != 4)turns--;
    steps--;//如果搜到底了，递归回来
  }
}

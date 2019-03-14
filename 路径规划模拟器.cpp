//路径规划模拟器 1.3 beta
/*
实现功能：提供小车在货架与货架之间的路径解决方案
*/
//by czl
//2019/03/14/23:20

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
int flag0;//标记障碍物位置
int m, n, x, y,a,b;//分别是开始点和结束点和过程坐标
int flag1=0;//记录所走步数
int flag2=0;//记录转弯数
int flag3[20];//记录每一步的前进方向，初始值0，上1，右2，下3，左4，这个数组也是从1开始计数的
int number=1;//记录现在试探到第几步,从1开始计数
int best = 100;//记录最好的成绩
int bestroad[20];//保存最优路径
int beststep;//保存最优步数

void shitan();


//-------(￣▽￣)／程序开始---------
int main() {
	memset(flag3, 0, sizeof(flag3));
	scanf_s("%d %d", &m, &n);//输入开始点
	scanf_s("%d %d", &x, &y);//输入结束点
	scanf_s("%d", &flag0);//输入障碍点

	switch (flag0) {//分情况给障碍物点赋值
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
	
	
	return 0;
}


void shitan() {
	if (a == x && b == y) {
		if (flag1 + flag2 < best) {
			for (int k = 1; k < number; k++) 
				bestroad[k] = flag3[k];
			best = flag1 + flag2;
			beststep = number;
		}
	}

	if (abs(a - x) + abs(b - y) > 15 && flag1 == 5)return;
	if (abs(a - x) + abs(b - y) > 12 && flag1 == 8)return;
	if (abs(a - x) + abs(b - y) > 8 && flag1 == 12)return;
	if (abs(a - x) + abs(b - y) > 5 && flag1 == 15)return;
	if (flag1 > 18)return;

	if (flag3[number-1] != 3&&spot[a][b-1]!=1) {//如果上一步没有向下走，而且上面没有障碍物，那么这步就可以向上走（否则就返回了）
		flag1++;//步数+1
		if (flag3[number - 1] != 1)flag2++;//如果上一步没有向上走，那么就转了一次弯
		b -= 1;//走了一步
		flag3[number] = 1;
		number += 1;
		shitan();
		number -= 1;
		b += 1;
		if (flag3[number - 1] != 1)flag2--;
		flag1--;//如果搜到底了，递归回来
	}
	if (flag3[number - 1] != 4 && spot[a+1][b] != 1) {//如果上一步没有向左走，而且右面没有障碍物，那么这步就可以向右走（否则就返回了）
		flag1++;//步数+1
		if (flag3[number - 1] != 2)flag2++;//如果上一步没有向右走，那么就转了一次弯
		a+= 1;//走了一步
		flag3[number] = 2;
		number += 1;
		shitan();
		number -= 1;
		a-= 1;
		if (flag3[number - 1] != 2)flag2--;
		flag1--;//如果搜到底了，递归回来
	}
	if (flag3[number - 1] != 1 && spot[a][b +1] != 1) {//如果上一步没有向上走，而且下面没有障碍物，那么这步就可以向下走（否则就返回了）
		flag1++;//步数+1
		if (flag3[number - 1] != 3)flag2++;//如果上一步没有向下走，那么就转了一次弯
		b += 1;//走了一步
		flag3[number] = 3;
		number += 1;
		shitan();
		number -= 1;
		b -= 1;
		if (flag3[number - 1] != 3)flag2--;
		flag1--;//如果搜到底了，递归回来
	}
	if (flag3[number - 1] != 2 && spot[a-1][b] != 1) {//如果上一步没有向右走，而且左面没有障碍物，那么这步就可以向左走（否则就返回了）
		flag1++;//步数+1
		if (flag3[number - 1] != 4)flag2++;//如果上一步没有向左走，那么就转了一次弯
		a -= 1;//走了一步
		flag3[number]=4;
		number += 1;
		shitan();
		number -= 1;
		a += 1;
		if (flag3[number - 1] != 4)flag2--;
		flag1--;//如果搜到底了，递归回来
	}
}

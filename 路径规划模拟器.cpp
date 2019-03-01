//路径规划模拟器 1.1 beta
/*
实现功能：在地图中快速规划路线
*/
//by czl
//2019/02/29/21:46

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
int row;//排
int line;//列
//对几种情况的不同方案
void Type1(int,int);
void Type2(int,int);
void Type3(int,int);
void Type4(int,int);
void Type5(int,int);
void Type6(int,int);
void Type7(int,int);
void Type8(int,int);
void Type14(int, int);
void print(int,int);//打印坐标

//-------(￣▽￣)／程序开始---------
int main() {
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
	row = m - x;
	line = n - y;//计算横纵坐标差
	a = m;
	b = n;//开始在出发点

	/*讨论十六种不同可能*/
	if (row >= 0 && line >= 0 && abs(row) >= abs(line)&&(n+y)<=11)
		Type1(a,b);
	else if (row >= 0 && line >= 0 && abs(row) >= abs(line)&&(n+y)>11)
		Type2(a,b);
	else if (row < 0 && line < 0 && abs(row) >= abs(line)&& (n + y) <= 11)
		Type3(a,b);
	else if (row < 0 && line < 0 && abs(row) >= abs(line)&&(n+y)>11)
		Type4(a,b);
	else if (row >= 0 && line >= 0 && abs(row) < abs(line)&&(m+x)<=11)
		Type5(a,b);
	else if (row >= 0 && line >= 0 && abs(row) < abs(line)&&(m+x)>11)
		Type6(a,b);
	else if (row < 0 && line < 0 && abs(row) < abs(line)&&(m+x)<=11)
		Type7(a,b);
	else if (row < 0 && line < 0 && abs(row) < abs(line)&&(m+x)>11)
		Type8(a,b);
	else if (row >= 0 && line < 0 && abs(row) >= abs(line) && (n+y)<=11)
		Type1(a, b);
	else if (row >= 0 && line < 0 && abs(row) >= abs(line) && (n+y)>11)
		Type2(a, b);
	else if (row < 0 && line >= 0 && abs(row) >= abs(line) && (n+y)<=11)
		Type3(a, b);
	else if (row < 0 && line >= 0 && abs(row) >= abs(line) && (n+y)>11)
		Type4(a, b);
	else if (row >= 0 && line < 0 && abs(row) < abs(line) && (m + x)<=11)
		Type5(a, b);
	else if (row >= 0 && line < 0 && abs(row) < abs(line) && (m + x)>11)
		Type14(a, b);
	else if (row < 0 && line >= 0 && abs(row) < abs(line) && (m + x)<=11)
		Type7(a, b);
	else if (row < 0 && line >= 0 && abs(row) < abs(line) && (m + x)>11)
		Type8(a, b);
	
	return 0;
}

void Type1(int a,int b) {
	if (a == x && b == y) {//到达结束点后退出递归
		printf("到达！\n");
		return;
	}
	if (a > x) {
		if (spot[a - 1][b] == 1) {
			b -= 1;
		}
		else {
			a -= 1;
		}
		print(a,b);
		Type1(a, b);
		return;
	}
	else if (b < y) {
		if (spot[a][b + 1] == 1) {
			a -= 1;
		}
		else {
			b += 1;
		}
		print(a,b);
		Type1(a, b);
		return;
	}
	else if (b > y) {
		if (spot[a][b - 1] == 1) {
			a -= 1;
		}
		else {
			b -= 1;
		}
		print(a, b);
		Type1(a, b);
		return;
	}
	else if (b==y) {
		a += 1;
		print(a,b);
		Type1(a, b);
		return;
	}

	return;
}

void Type2(int a, int b) {
	if (a == x && b == y) {//到达结束点后退出递归
		printf("到达！\n");
		return;
	}
	if (a > x) {
		if (spot[a - 1][b] == 1) {
			b += 1;
		}
		else {
			a -= 1;
		}
		print(a, b);
		Type2(a, b);
		return;
	}
	else if (b > y) {
		if (spot[a][b - 1] == 1) {
			a -= 1;
		}
		else {
			b -= 1;
		}
		print(a, b);
		Type2(a, b);
		return;
	}
	else if (b < y) {
		if (spot[a][b + 1] == 1) {
			a -= 1;
		}
		else {
			b += 1;
		}
		print(a, b);
		Type2(a, b);
		return;
	}
	else if (b == y) {
		a += 1;
		print(a, b);
		Type2(a, b);
		return;
	}

	return;
}

void Type3(int a, int b) {
	if (a == x && b == y) {//到达结束点后退出递归
		printf("到达！\n");
		return;
	}
	if (a < x) {
		if (spot[a + 1][b] == 1) {
			b -= 1;
		}
		else {
			a += 1;
		}
		print(a, b);
		Type3(a, b);
		return;
	}
	else if (b < y) {
		if (spot[a][b + 1] == 1) {
			a += 1;
		}
		else {
			b += 1;
		}
		print(a, b);
		Type3(a, b);
		return;
	}
	else if (b > y) {
		if (spot[a][b - 1] == 1) {
			a += 1;
		}
		else {
			b -= 1;
		}
		print(a, b);
		Type3(a, b);
		return;
	}
	else if (b == y) {
		a -= 1;
		print(a, b);
		Type3(a, b);
		return;
	}

	return;
}

void Type4(int a, int b) {
	if (a == x && b == y) {//到达结束点后退出递归
		printf("到达！\n");
		return;
	}
	if (a < x) {
		if (spot[a + 1][b] == 1) {
			b += 1;
		}
		else {
			a += 1;
		}
		print(a, b);
		Type4(a, b);
		return;
	}
	else if (b < y) {
		if (spot[a][b + 1] == 1) {
			a += 1;
		}
		else {
			b += 1;
		}
		print(a, b);
		Type4(a, b);
		return;
	}
	else if (b > y) {
		if (spot[a][b - 1] == 1) {
			a += 1;
		}
		else {
			b -= 1;
		}
		print(a, b);
		Type4(a, b);
		return;
	}
	else if (b == y) {
		a -= 1;
		print(a, b);
		Type4(a, b);
		return;
	}

	return;
}

void Type5(int a, int b) {
	if (a == x && b == y) {//到达结束点后退出递归
		printf("到达！\n");
		return;
	}
	if (b >y) {
		if (spot[a][b-1] == 1) {
			a-= 1;
		}
		else {
			b-= 1;
		}
		print(a, b);
		Type5(a, b);
		return;
	}
	else if (a < x) {
		if (spot[a+1][b] == 1) {
			b-= 1;
		}
		else {
			a += 1;
		}
		print(a, b);
		Type5(a, b);
		return;
	}
	else if (a > x) {
		if (spot[a-1][b] == 1) {
			b-= 1;
		}
		else {
			a -= 1;
		}
		print(a, b);
		Type5(a, b);
		return;
	}
	else if (a == x) {
		b+= 1;
		print(a, b);
		Type5(a, b);
		return;
	}

	return;
}

void Type6(int a, int b) {
	if (a == x && b == y) {//到达结束点后退出递归
		printf("到达！\n");
		return;
	}
	if (b >y) {
		if (spot[a][b - 1] == 1) {
			a -= 1;
		}
		else {
			b -= 1;
		}
		print(a, b);
		Type6(a, b);
		return;
	}
	else if (a < x) {
		if (spot[a + 1][b] == 1) {
			b -= 1;
		}
		else {
			a += 1;
		}
		print(a, b);
		Type6(a, b);
		return;
	}
	else if (a > x) {
		if (spot[a - 1][b] == 1) {
			b -= 1;
		}
		else {
			a -= 1;
		}
		print(a, b);
		Type6(a, b);
		return;
	}
	else if (a == x) {
		b += 1;
		print(a, b);
		Type6(a, b);
		return;
	}

	return;
}

void Type7(int a, int b) {
	if (a == x && b == y) {//到达结束点后退出递归
		printf("到达！\n");
		return;
	}
	if (b <y) {
		if (spot[a][b + 1] == 1) {
			//printf("spot[%d][%d]=%d",a,b+1, spot[a][b + 1]);
			a -= 1;
		}
		else {
			b += 1;
		}
		print(a, b);
		Type7(a, b);
		return;
	}
	else if (a < x) {
		if (spot[a + 1][b] == 1) {
			b += 1;
		}
		else {
			a += 1;
		}
		print(a, b);
		Type7(a, b);
		return;
	}
	else if (a > x) {
		if (spot[a - 1][b] == 1) {
			b += 1;
		}
		else {
			a -= 1;
		}
		print(a, b);
		Type7(a, b);
		return;
	}
	else if (a == x) {
		b -= 1;
		print(a, b);
		Type7(a, b);
		return;
	}

	return;
}

void Type8(int a, int b) {
	if (a == x && b == y) {//到达结束点后退出递归
		printf("到达！\n");
		return;
	}
	if (b <y) {
		if (spot[a][b + 1] == 1) {
			a += 1;
		}
		else {
			b += 1;
		}
		print(a, b);
		Type8(a, b);
		return;
	}
	else if (a < x) {
		if (spot[a + 1][b] == 1) {
			b += 1;
		}
		else {
			a += 1;
		}
		print(a, b);
		Type8(a, b);
		return;
	}
	else if (a > x) {
		if (spot[a - 1][b] == 1) {
			b += 1;
		}
		else {
			a -= 1;
		}
		print(a, b);
		Type8(a, b);
		return;
	}
	else if (a == x) {
		b -= 1;
		print(a, b);
		Type8(a, b);
		return;
	}

	return;
}

void Type14(int a, int b) {
	if (a == x && b == y) {//到达结束点后退出递归
		printf("到达！\n");
		return;
	}
	if (b <y) {
		if (spot[a][b + 1] == 1) {
			a += 1;
		}
		else {
			b += 1;
		}
		print(a, b);
		Type14(a, b);
		return;
	}
	else if (a < x) {
		if (spot[a + 1][b] == 1) {
			b -= 1;
		}
		else {
			a += 1;
		}
		print(a, b);
		Type14(a, b);
		return;
	}
	else if (a > x) {
		if (spot[a - 1][b] == 1) {
			b -= 1;
		}
		else {
			a -= 1;
		}
		print(a, b);
		Type14(a, b);
		return;
	}
	else if (a == x) {
		b -= 1;
		print(a, b);
		Type14(a, b);
		return;
	}

	return;
}


void print(int a,int b) {
	printf("[%d,%d]\n", a, b);
}

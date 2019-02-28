//路径规划模拟器 1.0 beta
//by czl
//2019/02/28/22:20

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <winnt.h>
#include <iostream>
/*全局函数与变量声明✧(≖ ◡ ≖✿ */
void print();//用来输出经过的点
int spot[12][12] = { {1,0,0,0,1,1,1,1,1,1,1,1},{1,666,0,0,0,666,666,666,666,666,666,1},{1,666,2333,0,0,0,2333,0,0,2333,0,1},{1,666,0,0,0,0,0,0,0,0,0,1},{1,666,0,0,1,1,1,1,0,0,0,1},{1,666,2333,0,1,1,1,1,0,0,666,1},{1,666,0,0,1,1,1,1,0,2333,666,1},{1,0,0,0,1,1,1,1,0,0,666,1},{1,0,0,0,0,0,0,0,0,0,666,1},{1,0,0,0,0,0,0,0,0,2333,666,1},{1,0,0,0,0,0,0,0,0,0,666,1},{1,1,1,1,1,1,1,1,0,0,0,1} };//0为可以到达的点，1为不可到达的点（包括货架与障碍物），待检查的障碍物可能点记为2333，待检查ABCD货架点为666,上层有空位则为6，下层有空则为8
int flag0;//标记障碍物位置
int scanner[20];//一个用来放乱七八糟东西的数组
int m = 1, n = 8;//开始点
int thing[9];//保存E货架上的物体
int carbag[2] = { 0,0 };//暂定车上只能放两个物体
//-------(￣▽￣)／程序开始---------
int main() {
	scanf_s("%d %d %d\n", &scanner[0], &scanner[1], &scanner[2]);
	scanf_s("%d %d %d\n", &scanner[3], &scanner[4], &scanner[5]);
	scanf_s("%d %d %d\n", &scanner[6], &scanner[7], &scanner[8]);
	spot[scanner[0]][scanner[1]] = scanner[2];
	spot[scanner[3]][scanner[4]] = scanner[5];
	spot[scanner[6]][scanner[7]] = scanner[8];
	m += 1;//向左进一步
	print();
	printf("开始检测左方两个障碍物（若有障碍物输入它的编号，若无障碍物，则输入0）：\n");
	scanf_s("%d", &flag0);//如果有障碍物输入它的编号，若无障碍物，则输入0
	m += 1;
	print();
	m += 1;
	print();
	m += 1;
	print();
	printf("检测1号台上的物体（输入三个物体的编号）：\n");
	scanf_s("%d %d %d", &thing[0], &thing[1], &thing[2]);
	if (thing[0] <= 3) {
		printf("抓取左边的物体\n");
		carbag[0] = thing[0];
	}
	if (thing[1] <= 3) {
		printf("抓取中间的物体\n");
		if (carbag[0] == 0)carbag[0] = thing[1];
		else carbag[1] = thing[1];
	}
	if (thing[2] <= 3 && carbag[1] == 0) {
		printf("抓取右边的物体\n");
		if (carbag[0] == 0)carbag[0] = thing[2];
		else carbag[1] = thing[2];
	}
	m += 1;
	print();
	m += 1;
	print();
	printf("右转\n");
	n += 1;
	print();
	printf("开始检测左方一个障碍物（若有障碍物输入它的编号，若无障碍物，则输入0）：\n");
	scanf_s("%d", &flag0);//如果有障碍物输入它的编号，若无障碍物，则输入0
	printf("左转\n");
	m += 1;
	print();
	printf("右转\n");

}
void print() {
	printf("[%d][%d]\n", m, n);//输出此刻所在的点
}

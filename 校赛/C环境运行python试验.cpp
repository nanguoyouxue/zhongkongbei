#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <winnt.h>
#include <iostream>
#include <time.h>
#include<stdbool.h>
#include<pthread.h>        //包含多线程支持库头文件
#pragma comment(lib, "pthreadVC2.lib")//这句话别忘了

using namespace std;

pthread_t t1;                           //pthread_t变量t1，用于存储线程ID
int drink;

void* My_thread(void* args) {            //线程运行函数
	clock_t start = clock();
	system("python E:\\lalala\\classify_image_inception_v3.py");
	clock_t end = (clock() - start) / CLOCKS_PER_SEC;
	cout << "time comsumption is " << end << endl;
	return NULL;
}

int main() {                              //主程序
	pthread_create(&t1, NULL, My_thread, NULL);  //创建缺省线程
	printf_s("我是主线程\n");
	Sleep(1000);
	printf_s("我真的是主线程\n");
	Sleep(9000);
	printf_s("我还是主线程\n");
	FILE *fp;//开始读写的句柄
	fopen_s(&fp, "predict_result.txt", "r+");
	fscanf_s(fp, "%d", &drink);
	if (drink == 1)printf_s("AD钙奶");
	else if (drink == 2)printf_s("红牛");
	else if (drink == 3)printf_s("雪花啤酒");
	fclose(fp);
	fopen_s(&fp,"predict_result.txt", "w+");
	fclose(fp);
	return 0;
}

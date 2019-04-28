#include "stdafx.h"

#include<stdio.h>
#include<stdbool.h>
#include<pthread.h>        //包含多线程支持库头文件
#include<Windows.h>
#pragma comment(lib, "pthreadVC2.lib")//这句话别忘了

pthread_t t1;                           //pthread_t变量t1，用于存储线程ID

void* My_thread(void* args) {            //线程运行函数
	for (int i = 0; i<100; i++) {
		printf_s("I am your thread\n");
	}
	return NULL;
}

int main() {                              //主程序
	pthread_create(&t1, NULL, My_thread, NULL);  //创建缺省线程
	for (int i = 0; i<100; i++) {
		printf("hello world!\n");
	}
	return 0; 
}

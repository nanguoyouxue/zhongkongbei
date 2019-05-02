/*训练集收集器 1.1
打开以后按空格就可以保存图片
by czl
2019/04/29
*/

#include "stdafx.h"
#include<stdio.h>
#include<stdbool.h>
#include<conio.h>
#include<pthread.h>        //包含多线程支持库头文件
#include<Windows.h>
#include <iostream>
#include <conio.h>
#pragma comment(lib, "pthreadVC2.lib")

#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

Mat frame;//保存像素的二维张量
char str[10];
int i=0;

int main() {
	VideoCapture capture(0);//摄像头初始化
	Sleep(30);//等待30ms摄像头真正开启

	while (1) {
		capture >> frame;//将摄像头的一帧赋值给变量

						 //照片预处理
		transpose(frame, frame);
		flip(frame, frame, 1);//逆时针旋转270°
		Rect area(60, 0, 360, 640);//前两个值是起点坐标，后两个是横竖的长度
		frame = frame(area);//裁剪照片去黑边

		imshow("拍照", frame);//将这张照片显示出来
		waitKey(100);//等待30ms

		//FILE *fp;读写文件的示例
		//fopen_s(&fp, "train.txt", "r");
		if (cvWaitKey(10) >= 0) {
			sprintf_s(str, "%d.jpg", i);
			imwrite(str, frame);//将这张照片保存为1.jpg
			imshow("图片", frame);//将这张照片显示出来
			printf_s("已保存：%s\n", str);
			i++;
			Sleep(1000);
		}

     }
		//fclose(fp);

	return 0;//主程序结束
}

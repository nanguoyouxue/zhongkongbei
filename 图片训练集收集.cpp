/*图片训练集收集器 1.0
by czl
2019/04/29
*/

#include "stdafx.h"
#include<stdio.h>
#include<stdbool.h>
#include<pthread.h>        //包含多线程支持库头文件
#include<Windows.h>
#pragma comment(lib, "pthreadVC2.lib")

#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

int main() {
	VideoCapture capture(0);//摄像头初始化
	
	Sleep(30);//等待30ms摄像头真正开启
	Mat frame;//保存像素的二维数组
	capture >> frame;//将摄像头的一帧赋值给变量

	//照片预处理
	transpose(frame, frame);
	flip(frame, frame, 1);//逆时针旋转270°
	Rect area(60, 0, 360, 640);//前两个值是起点坐标，后两个是横竖的长度
	frame=frame(area);//裁剪照片去黑边

	imwrite("1.jpg", frame);//将这张照片保存为1.jpg
	imshow("图片", frame);//将这张照片显示出来
	waitKey(0);//直到任意按键按下，此次程序结束

	return 0;//主程序结束
}

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

#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
using namespace cv;
using namespace std;
/*全局函数与变量声明✧(≖ ◡ ≖✿ */

//机械臂串口定义
DWORD dwLength;//输入缓冲区的大小（字节数）
char recvBuf[60];//输入的信息保存在该字符串中
DWORD dwactlen;//输出缓冲区的大小（字节数）
char psendbuf[99];//输出的信息保存在该字符串中
DCB myDCB;//初始化串口配置信息DCB结构
HANDLE m_hComm;//串口句柄，定义串口变量

			   //下位机串口定义
DWORD dwLength0;//输入缓冲区的大小（字节数）
char recvBuf0[10];//输入的信息保存在该字符串中
DWORD dwactlen0;//输出缓冲区的大小（字节数）
char psendbuf0[99];//输出的信息保存在该字符串中
DCB myDCB0;//初始化串口配置信息DCB结构
HANDLE m_hComm0;//串口句柄，定义串口变量

				//机械臂相关函数
bool COM3();//COM3（机械臂）接口初始化函数
void hello();//接收机械臂arduino板打招呼信息
void arm(char *);//向机械臂发送数据

				 //下位机相关函数
bool COM5();//COM5（下位机）接口初始化函数
void tance();//接收来自arduino的命令并执行
void Close();//关闭串口等程序关闭前的准备工作

			 //全局变量
Mat frame;//保存像素的二维张量
char str[10];
int picname = 0;
int spot[12][12] = { { 1,1,1,1,1,1,1,1,1,1,1,1 },{ 1,0,0,0,0,0,0,0,0,0,0,1 },{ 1,0,0,0,0,0,0,0,0,0,0,1 },{ 1,0,0,0,0,0,0,0,0,0,0,1 },{ 1,0,0,0,1,1,1,1,0,0,0,1 },{ 1,0,0,0,1,1,1,1,0,0,0,1 },{ 1,0,0,0,1,1,1,1,0,0,0,1 },{ 1,0,0,0,1,1,1,1,0,0,0,1 },{ 1,1,0,0,0,0,0,0,0,0,0,1 },{ 1,0,0,0,0,0,0,0,0,0,0,1 },{ 1,0,0,0,0,0,0,0,0,0,0,1 },{ 1,1,1,1,1,1,1,1,1,1,1,1 } };//0为可以到达的点，1为不可到达的点（包括货架与障碍物）
int barrier;//标记障碍物位置
int m, n, x, y, a, b;//分别是开始点和结束点和过程坐标
int steps = 0;//记录所走步数
int turns = 0;//记录转弯数
int direction[20];//记录每一步的前进方向，初始值0，上1，右2，下3，左4，这个数组也是从1开始计数的
int number = 1;//记录现在试探到第几步,从1开始计数
int best = 100;//记录最好的成绩
int bestroad[20];//保存最优路径
int beststep;//保存最优步数
char lines[20];//保存路径规划的串口指令
int counts;//路径规划中计数
int i, j;//计数变量
int p = 1;//路径规划相关
void shitan();//路径规划
char drink[20];
int flag8 = 1;//视觉识别标识符
Mat image_src;
Mat imageROI;
Mat TempImg;
errno_t err;



//子线程
pthread_t t1;                           //pthread_t变量t1，用于存储线程ID
bool endflag = 1;

//色相机准备
VideoCapture capture(0);//摄像头初始化

void* My_thread(void* args) {            //子线程函数
										 //clock_t start = clock();
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-1.py");//1
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-2.py");//2
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-3.py");//3
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-4.py");//4
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-5.py");//5
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-6.py");//6
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-7.py");//7
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-8.py");//8
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-9.py");//9
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-10.py");//10
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-11.py");//11
	flag8++;
	while (picname <= flag8) {
		Sleep(1000);
	}
	system("python E:\\lalala\\classify_image_inception_v3-12.py");//12
	flag8++;
	//clock_t end = (clock() - start) / CLOCKS_PER_SEC;//计时器
	//cout << "time comsumption is " << end << endl;
	return NULL;
}

//-------(￣▽￣)／程序开始---------
int main() {
	printf("删除所有照片！！！！\n");
	//串口准备
	if (COM3() == FALSE) return 0;//COM3（机械臂）接口初始化
	hello();//接收打招呼信息，准备完毕

	if (COM5() == FALSE) return 0;//COM5（下位机）接口初始化
	pthread_create(&t1, NULL, My_thread, NULL);  //创建缺省线程
												 //开始工作
	while (endflag == 1)tance();//开始接收arduino发出的指令

	Close();
	return 0;
}

bool COM3() {
loop:m_hComm = CreateFile(//打开端口
	"COM4:",//机械臂使用COM3端口
	GENERIC_READ | GENERIC_WRITE,  //允许读和写
	0,                           //独占方式（共享模式）此处必须独占
	NULL,
	OPEN_EXISTING,              //打开而不是创建（创建方式）
	0,
	NULL
);
	 if (m_hComm == (HANDLE)-1) {                          //打开串口失败返回
		 printf("打开机械臂串口失败--->请检查USB是否正确连接！！！\n尝试重连中……\n");
		 Sleep(1000);//延时1s
		 goto loop;//返回尝试再次打开串口
		 return FALSE;
	 }
	 printf("打开机械臂串口成功！\n");
	 //得到打开串口的当前属性参数，修改后再重新设置串口。


	 if (!GetCommState(m_hComm, &myDCB)) {
		 printf("无法获取机械臂串口当前属性\n");
		 return FALSE;
	 }


	 //设置串口参数  (这段话没弄懂前不要修改，除了波特率）
	 myDCB.BaudRate = CBR_115200;   // 设置波特率115200
	 myDCB.fBinary = TRUE; // 设置二进制模式，此处必须设置TRUE
	 myDCB.fParity = FALSE; // 不支持奇偶校验
	 myDCB.fOutxCtsFlow = FALSE;  // 指定CTS不用于检测发送流控制
	 myDCB.fOutxDsrFlow = FALSE;  //   指定DSR不用于检测发送流控制
	 myDCB.fDtrControl = DTR_CONTROL_DISABLE; // 指定DTR不进行流量控制
	 myDCB.fDsrSensitivity = FALSE; // 指定通信驱动程序对DTR信号线是否敏感，如果该位置设为TRUE时，DSR信号为OFF，接收的任何字节将被忽略。
	 myDCB.fTXContinueOnXoff = TRUE; //  指定当接收缓冲区已满，并且驱动程序已经发送出XoffChar字符时发送是否停止。
									 //当该成员为TRUE时，在接收缓冲区内接收到了缓冲区已满的字节XoffLim，并且驱动程序已经发送出XoffChar字符终止接收字节之后，发送继续进行。
									 //该成员为FALSE时，接收缓冲区接收到代表缓冲区已空的字节XonLim，并且驱动程序已经发送出恢复发送的XonChar字符后，发送可以继续进行。
	 myDCB.fOutX = FALSE;     //不用XoffChar进行发送控制，该成员为TRUE时，接收到XoffChar之后停止发送，接收到XonChar之后发送将重新开始。
	 myDCB.fInX = FALSE;        //该成员为TRUE时，接收缓冲区内接收到代表缓冲区满的字节XoffLim之后，XoffChar发送出去，
								//接收缓冲区接收到代表缓冲区已空的字节XonLim之后，XonChar发送出去。
	 myDCB.fErrorChar = FALSE;    //当该成员与奇偶校验同时开启时，就会用ErrorChar成员指定的字符来代替奇偶校验错误的接收字符。
	 myDCB.fNull = FALSE;  //接收值为NULL的输入数据
	 myDCB.fRtsControl = RTS_CONTROL_DISABLE;   //指定RTS不进行流量控制
	 myDCB.fAbortOnError = FALSE;  // 当串口发生错误，不终止串口读写
	 myDCB.ByteSize = 8;      //数据位,范围:4-8
	 myDCB.Parity = NOPARITY; //无奇偶校验模式
	 myDCB.StopBits = 0;   //1位停止位

						   //如果串口参数修改失败，返回FALSE

	 if (!SetCommState(m_hComm, &myDCB)) {
		 printf("设置机械臂串口失败\n");
		 return FALSE;
	 }

	 SetupComm(m_hComm, 1024, 1024);//设置串口的输入/输出缓冲区大小
	 PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);//清空缓冲区，作为初始化

													   //超时的作用是在指定的时间内没有读入或发送指定数量的字符，ReadFile或WriteFile的操作仍然会结束。
													   //超时设置
	 COMMTIMEOUTS TimeOuts;
	 //设定读超时
	 TimeOuts.ReadIntervalTimeout = 1000;//读间隔超时(指在接收时两个字符之间的最大时延)
	 TimeOuts.ReadTotalTimeoutMultiplier = 100;//读时间系数
	 TimeOuts.ReadTotalTimeoutConstant = 10;//读时间常量
											//设定写超时（写没有间隔超时函数）
	 TimeOuts.WriteTotalTimeoutMultiplier = 50;//写时间系数
	 TimeOuts.WriteTotalTimeoutConstant = 20;//写时间常量
											 //总超时的计算
											 //总超时＝时间系数×要求读/写的字符数＋时间常量
											 //例如，要读入10个字符，那么读操作的总超时的计算公式为：
											 //读总超时＝ReadTotalTimeoutMultiplier×10＋ReadTotalTimeoutConstant

	 SetCommTimeouts(m_hComm, &TimeOuts); //将设置好的超时写入串口
	 return TRUE;
}

void hello() {
	//先读取两次（机械臂Arduino在串口连接成功后会先发送打招呼信息）
	//第一次读取
	ReadFile(
		m_hComm, //串口名称
		recvBuf, //保存输入信息的数组
		60, //输入的数据长度（这个值相当于限制输入数据的最长长度）
		&dwLength, //实际输入的数据长度
		NULL//该值必须为NULL
	);
	printf("读取机械臂信息中……\n");//成功读取打招呼内容后输出提示信息
	PurgeComm(m_hComm, PURGE_RXCLEAR);//清空输入寄存器
									  //第二次读取
	ReadFile(m_hComm, recvBuf, 60, &dwLength, NULL);
	printf_s("收到机械臂发送的数据：\n%-60.60s\n", recvBuf);//%-60.60s表示限制输出字符个数为60个
	PurgeComm(m_hComm, PURGE_RXCLEAR);
	printf("开始发送机械臂舵机指令……\n");
}

void arm(char *psendbuf) {
	WriteFile(m_hComm, psendbuf, 99, &dwactlen, NULL);
	PurgeComm(m_hComm, PURGE_TXCLEAR);//每次发完指令都要清空输出寄存器
	Sleep(1800);//延时1.8秒，延时是为了等待机械臂完成指令以后再发出下一个指令（中间还有电脑与机械臂传输数据，处理数据的时间），可根据实际情况调整
}

bool COM5() {
loop:m_hComm0 = CreateFile(//打开端口
	"COM9:",//下位机使用COM5端口
	GENERIC_READ | GENERIC_WRITE,  //允许读和写
	0,                           //独占方式（共享模式）此处必须独占
	NULL,
	OPEN_EXISTING,              //打开而不是创建（创建方式）
	0,
	NULL
);
	 if (m_hComm0 == (HANDLE)-1) {                          //打开串口失败返回
		 printf("打开下位机串口失败--->请检查USB是否正确连接！！！\n尝试重连中……\n");
		 Sleep(1000);//延时1s
		 goto loop;//返回尝试再次打开串口
		 return FALSE;
	 }
	 printf("打开下位机串口成功！\n");
	 //得到打开串口的当前属性参数，修改后再重新设置串口。


	 if (!GetCommState(m_hComm0, &myDCB0)) {
		 printf("无法获取下位机串口当前属性\n");
		 return FALSE;
	 }


	 //设置串口参数  (这些设置除波特率以外一般无需修改）
	 myDCB0.BaudRate = CBR_9600;   // 设置波特率为9600
	 myDCB0.fBinary = TRUE; // 设置二进制模式，此处必须设置TRUE
	 myDCB0.fParity = FALSE; // 不支持奇偶校验
	 myDCB0.fOutxCtsFlow = FALSE;  // 指定CTS不用于检测发送流控制
	 myDCB0.fOutxDsrFlow = FALSE;  //   指定DSR不用于检测发送流控制
	 myDCB0.fDtrControl = DTR_CONTROL_DISABLE; // 指定DTR不进行流量控制
	 myDCB0.fDsrSensitivity = FALSE; // 指定通信驱动程序对DTR信号线是否敏感，如果该位置设为TRUE时，DSR信号为OFF，接收的任何字节将被忽略。
	 myDCB0.fTXContinueOnXoff = TRUE; //  指定当接收缓冲区已满，并且驱动程序已经发送出XoffChar字符时发送是否停止。
									  //当该成员为TRUE时，在接收缓冲区内接收到了缓冲区已满的字节XoffLim，并且驱动程序已经发送出XoffChar字符终止接收字节之后，发送继续进行。
									  //该成员为FALSE时，接收缓冲区接收到代表缓冲区已空的字节XonLim，并且驱动程序已经发送出恢复发送的XonChar字符后，发送可以继续进行。
	 myDCB0.fOutX = FALSE;     //不用XoffChar进行发送控制，该成员为TRUE时，接收到XoffChar之后停止发送，接收到XonChar之后发送将重新开始。
	 myDCB0.fInX = FALSE;        //该成员为TRUE时，接收缓冲区内接收到代表缓冲区满的字节XoffLim之后，XoffChar发送出去，
								 //接收缓冲区接收到代表缓冲区已空的字节XonLim之后，XonChar发送出去。
	 myDCB0.fErrorChar = FALSE;    //当该成员与奇偶校验同时开启时，就会用ErrorChar成员指定的字符来代替奇偶校验错误的接收字符。
	 myDCB0.fNull = FALSE;  //接收值为NULL的输入数据
	 myDCB0.fRtsControl = RTS_CONTROL_DISABLE;   //指定RTS不进行流量控制
	 myDCB0.fAbortOnError = FALSE;  // 当串口发生错误，不终止串口读写
	 myDCB0.ByteSize = 8;      //数据位,范围:4-8
	 myDCB0.Parity = NOPARITY; //无奇偶校验模式
	 myDCB0.StopBits = 0;   //1位停止位

							//如果串口参数修改失败，返回FALSE

	 if (!SetCommState(m_hComm0, &myDCB0)) {
		 printf("设置下位机串口失败\n");
		 return FALSE;
	 }

	 SetupComm(m_hComm0, 1024, 1024);//设置串口的输入/输出缓冲区大小
	 PurgeComm(m_hComm0, PURGE_RXCLEAR | PURGE_TXCLEAR);//清空缓冲区，作为初始化

														//超时的作用是在指定的时间内没有读入或发送指定数量的字符，ReadFile或WriteFile的操作仍然会结束。
														//超时设置
	 COMMTIMEOUTS TimeOuts;
	 //设定读超时（---------注意：下位机arduino程序中有各种延时，务必计算仔细-------------)
	 TimeOuts.ReadIntervalTimeout = 1000;//读间隔超时(指在接收时两个字符之间的最大时延)
	 TimeOuts.ReadTotalTimeoutMultiplier = 500;//读时间系数
	 TimeOuts.ReadTotalTimeoutConstant = 1000;//读时间常量
											  //设定写超时（写没有间隔超时函数）
	 TimeOuts.WriteTotalTimeoutMultiplier = 50;//写时间系数
	 TimeOuts.WriteTotalTimeoutConstant = 20;//写时间常量
											 //总超时的计算
											 //总超时＝时间系数×要求读/写的字符数＋时间常量
											 //例如，要读入10个字符，那么读操作的总超时的计算公式为：
											 //读总超时＝ReadTotalTimeoutMultiplier×10＋ReadTotalTimeoutConstant

	 SetCommTimeouts(m_hComm0, &TimeOuts); //将设置好的超时写入串口
	 return TRUE;
}

void Close() {
	printf("机械臂指令发送完毕！\n");
	CloseHandle(m_hComm); //m_hComm是CreateFile函数返回的串口句柄，关闭串口，程序结束
	printf("机械臂串口关闭\n");

	CloseHandle(m_hComm0); //关闭串口，程序结束
	printf("下位机串口关闭\n");
}

void tance() {
	printf("读取中……\n");

	ReadFile(m_hComm0, recvBuf0, 40, &dwLength0, NULL);
	printf("收到数据：%s\n", recvBuf0);//输出收到的信息
	PurgeComm(m_hComm, PURGE_RXCLEAR);//每次接收信息之后清空缓存区

									  //开始解析收到的数据内容
	if (recvBuf0[0] == 'M') {
		if (recvBuf0[1] == '5') {
			printf("开始拍照……\n");
			if (recvBuf0[2] == '1') {
				printf("Warnig：如果程序突然退出，请检查图片是否删除!!!\n");
				capture >> frame;//将摄像头的一帧赋值给变量
								 //照片裁剪
				transpose(frame, frame);
				flip(frame, frame, 1);//逆时针旋转270°
									  //保存图片
				image_src = frame.clone();
				imageROI = image_src(Rect(60, 280, 120, 150));
				imageROI.convertTo(TempImg, TempImg.type());
				sprintf_s(str, "%d.jpg", picname);
				imwrite(str, TempImg);//将这张照片保存为*.jpg
									  //imshow("图片", frame);//将这张照片显示出来
				printf_s("已保存：%s\n", str);
				picname++;
				image_src = frame.clone();
				imageROI = image_src(Rect(180, 280, 120, 150));
				imageROI.convertTo(TempImg, TempImg.type());
				sprintf_s(str, "%d.jpg", picname);
				imwrite(str, TempImg);//将这张照片保存为*.jpg
									  //imshow("图片", frame);//将这张照片显示出来
				printf_s("已保存：%s\n", str);
				picname++;
				image_src = frame.clone();
				imageROI = image_src(Rect(300, 280, 120, 150));
				imageROI.convertTo(TempImg, TempImg.type());
				sprintf_s(str, "%d.jpg", picname);
				imwrite(str, TempImg);//将这张照片保存为*.jpg
									  //imshow("图片", frame);//将这张照片显示出来
				printf_s("已保存：%s\n", str);
				picname++;
				strcpy_s(psendbuf0, "M6!");
				printf_s("向arduino发送：%s\n", psendbuf0);
				WriteFile(m_hComm0, psendbuf0, 10, &dwactlen0, NULL);
				PurgeComm(m_hComm0, PURGE_TXCLEAR);//每次发完指令都要清空输出寄存器
				memset(psendbuf0, 0, sizeof psendbuf0);//每次发完指令也要清空保存指令的字符串
			}
			if (recvBuf0[2] == '2') {
				capture >> frame;//将摄像头的一帧赋值给变量
								 //照片裁剪
				transpose(frame, frame);
				flip(frame, frame, 1);//逆时针旋转270°
									  //保存图片
				image_src = frame.clone();
				imageROI = image_src(Rect(60, 320, 120, 200));
				imageROI.convertTo(TempImg, TempImg.type());
				sprintf_s(str, "%d.jpg", picname);
				imwrite(str, TempImg);//将这张照片保存为*.jpg
									  //imshow("图片", frame);//将这张照片显示出来
				printf_s("已保存：%s\n", str);
				picname++;
				strcpy_s(psendbuf0, "M6!");
				printf_s("向arduino发送：%s\n", psendbuf0);
				WriteFile(m_hComm0, psendbuf0, 10, &dwactlen0, NULL);
				PurgeComm(m_hComm0, PURGE_TXCLEAR);//每次发完指令都要清空输出寄存器
				memset(psendbuf0, 0, sizeof psendbuf0);//每次发完指令也要清空保存指令的字符串
			}
			memset(recvBuf0, 0, sizeof recvBuf0);
		}
		else if (recvBuf0[1] == '6') {
			barrier = recvBuf0[2] - '0';
			if (recvBuf0[3] == 'Y') {
				switch (barrier) {//分情况给障碍物点赋值
				case 1:spot[9][9] = 1; break;
				case 2:spot[9][6] = 1; break;
				case 3:spot[9][2] = 1; break;
				case 4:spot[6][2] = 1; break;
				case 5:spot[2][2] = 1; break;
				case 6:spot[2][5] = 1; break;
				}
			}
			memset(recvBuf0, 0, sizeof recvBuf0);
		}
		else if (recvBuf0[1] == '7') {
			/*memset(recvBuf0, 0, sizeof recvBuf0);
			memset(drink, 0, sizeof drink);
			memset(drink, 0, sizeof drink);
			printf("111\n");
			FILE *fp;//开始读写的句柄
			printf("222\n");
			fopen_s(&fp, "predict_result.txt", "r+");//打开保存结果的文件
			printf("333\n");
			for (int k = 0; k < 12; k++) {
				fscanf_s(fp, "%c", &drink[k]);
			}
			printf("444\n");
			fclose(fp);*/

			//回复arduino
			//sprintf_s(psendbuf0, "M%s!", drink);
			//psendbuf0 = drink;
			strcpy_s(psendbuf0, "M212212212333!");//视觉识别暂时不加，先这样<<<<<<<<<<<<<<<<<<<<<<<
			printf_s("向arduino发送：%s\n", psendbuf0);
			WriteFile(m_hComm0, psendbuf0, 30, &dwactlen0, NULL);
			PurgeComm(m_hComm0, PURGE_TXCLEAR);//每次发完指令都要清空输出寄存器
			memset(psendbuf0, 0, sizeof psendbuf0);//每次发完指令也要清空保存指令的字符串
		}
		else if (recvBuf0[1] == '1') {
			steps = 0;//记录所走步数
			turns = 0;//记录转弯数
			number = 1;//记录现在试探到第几步,从1开始计数
			best = 100;//记录最好的成绩
			p = 1;
			memset(direction, 0, sizeof(direction));
			m = (recvBuf0[2] - '0') * 10 + (recvBuf0[3] - '0');
			n = (recvBuf0[4] - '0') * 10 + (recvBuf0[5] - '0');
			x = (recvBuf0[6] - '0') * 10 + (recvBuf0[7] - '0');
			y = (recvBuf0[8] - '0') * 10 + (recvBuf0[9] - '0');

			a = m;
			b = n;//开始在出发点

			shitan();

			//将bestroad翻译成串口语言
			lines[0] = 'M';
			lines[1] = '1';
			if (beststep == 2) { lines[3] = '0' + bestroad[1]; lines[4] = '1'; }
			else {
				for (i = 1, j = 3; i <= beststep - 1; i++)
				{
					if (i == 1) counts = 1;
					else if (i >= 2 && bestroad[i] == bestroad[i - 1]) counts++;
					else { lines[j] = '0' + bestroad[i - 1]; lines[j + 1] = '0' + counts; counts = 1; j = j + 2; p++; }
					if (i == beststep - 1) { lines[j] = '0' + bestroad[i]; lines[j + 1] = '0' + counts; }
				}
			}
			lines[2] = '0' + p;
			printf_s("向arduino发送数据:%s\n", lines);
			sprintf_s(psendbuf0, "%s!", lines);
			WriteFile(m_hComm0, psendbuf0, 20, &dwactlen0, NULL);
			PurgeComm(m_hComm0, PURGE_TXCLEAR);//每次发完指令都要清空输出寄存器
			memset(psendbuf0, 0, sizeof psendbuf0);//每次发完指令也要清空保存指令的字符串
			memset(recvBuf0, 0, sizeof recvBuf0);
		}
		else if (recvBuf0[1] == '8') {
			if (recvBuf0[2] == '0') {//未监修
				strcpy_s(psendbuf, "{G0000#000P2150T1000!#001P2100T1000!#002P2100T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);//拍照姿势
			}
			else if (recvBuf0[2] == '1') {//左边抓
				strcpy_s(psendbuf, "{G0000#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);//回到初始状态
				strcpy_s(psendbuf, "{G0001#000P1800T1000!#001P0900T1000!#002P0900T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0002#000P1800T1000!#001P0900T1000!#002P0900T1000!#003P1500T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0003#000P1800T1000!#001P1030T1000!#002P2250T1000!#003P900T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0004#000P1800T1000!#001P1500T1000!#002P2250T1000!#003P900T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0005#000P1800T1000!#001P0900T1000!#002P1800T1000!#003P91140T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0006#000P1800T1000!#001P0800T1000!#002P1700T1000!#003P1200T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0007#000P1800T1000!#001P0900T1000!#002P1550T1000!#003P1600T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0008#000P1500T1000!#001P0900T1000!#002P0900T1000!#003P1500T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
			}
			else if (recvBuf0[2] == '2') {//中间抓
				strcpy_s(psendbuf, "{G0000#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);//回到初始状态
				strcpy_s(psendbuf, "{G0001#000P1500T1000!#001P0900T1000!#002P0900T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0002#000P1500T1000!#001P0900T1000!#002P0900T1000!#003P1500T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0003#000P1500T1000!#001P1500T1000!#002P2250T1000!#003P900T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0004#000P1500T1000!#001P1030T1000!#002P2250T1000!#003P900T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0005#000P1500T1000!#001P0900T1000!#002P1800T1000!#003P91140T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0006#000P1500T1000!#001P0800T1000!#002P1700T1000!#003P1200T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0007#000P1500T1000!#001P0900T1000!#002P0900T1000!#003P1500T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
			}
			else if (recvBuf0[2] == '3') {//右边抓
				strcpy_s(psendbuf, "{G0000#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);//回到初始状态
				strcpy_s(psendbuf, "{G0001#000P1200T1000!#001P0900T1000!#002P0900T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0002#000P1200T1000!#001P0900T1000!#002P0900T1000!#003P1500T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0003#000P1200T1000!#001P1030T1000!#002P2250T1000!#003P900T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0004#000P1200T1000!#001P1500T1000!#002P2250T1000!#003P900T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0005#000P1200T1000!#001P0900T1000!#002P1800T1000!#003P91140T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0006#000P1200T1000!#001P0800T1000!#002P1700T1000!#003P1200T1000!#004P1500T1000!#005P2100T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0007#000P1200T1000!#001P0900T1000!#002P1550T1000!#003P1600T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0008#000P1500T1000!#001P0900T1000!#002P0900T1000!#003P1500T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
			}
			else if (recvBuf0[2] == '4') {//下面格子
				strcpy_s(psendbuf, "{G0001#000P1500T1000!#001P2100T1000!#002P2100T1000!#003P1500T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0002#000P1500T1000!#001P2100T1000!#002P2500T1000!#003P0800T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0003#000P1500T1000!#001P1350T1000!#002P2500T1000!#003P0800T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0004#000P1500T1000!#001P1250T1000!#002P2350T1000!#003P0800T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0005#000P1500T1000!#001P1150T1000!#002P2100T1000!#003P1100T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0006#000P1500T1000!#001P0900T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0007#000P1500T1000!#001P0900T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1800T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0008#000P1500T1000!#001P1150T1000!#002P2100T1000!#003P1100T1000!#004P1500T1000!#005P1800T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0009#000P1500T1000!#001P1250T1000!#002P2350T1000!#003P0800T1000!#004P1500T1000!#005P1800T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0010#000P1500T1000!#001P1350T1000!#002P2500T1000!#003P0800T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0011#000P1500T1000!#001P2100T1000!#002P2500T1000!#003P0800T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0012#000P1500T1000!#001P2100T1000!#002P2100T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);

			}
			else if (recvBuf0[2] == '5') {//上方格子
				strcpy_s(psendbuf, "{G0001#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1800T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0002#000P1500T1000!#001P1230T1000!#002P1500T1000!#003P1800T1000!#004P1500T1000!#005P1230T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0003#000P1500T1000!#001P1230T1000!#002P1500T1000!#003P1800T1000!#004P1500T1000!#005P1800T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0004#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1800T1000!#004P1500T1000!#005P1800T1000!}"); arm(psendbuf);
				strcpy_s(psendbuf, "{G0005#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);
			}
			memset(recvBuf0, 0, sizeof recvBuf0);
			strcpy_s(psendbuf0, "M6!");
			printf_s("向arduino发送：%s\n", psendbuf0);
			WriteFile(m_hComm0, psendbuf0, 10, &dwactlen0, NULL);
			PurgeComm(m_hComm0, PURGE_TXCLEAR);//每次发完指令都要清空输出寄存器
			memset(psendbuf0, 0, sizeof psendbuf0);//每次发完指令也要清空保存指令的字符串
		}
		else if (recvBuf0[1] == '9') {
			strcpy_s(psendbuf, "{G0000#000P0850T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!}"); arm(psendbuf);//超声波姿势
			memset(recvBuf0, 0, sizeof recvBuf0);
			strcpy_s(psendbuf0, "M6!");
			printf_s("向arduino发送：%s\n", psendbuf0);
			WriteFile(m_hComm0, psendbuf0, 10, &dwactlen0, NULL);
			PurgeComm(m_hComm0, PURGE_TXCLEAR);//每次发完指令都要清空输出寄存器
			memset(psendbuf0, 0, sizeof psendbuf0);//每次发完指令也要清空保存指令的字符串
		}
		else if (recvBuf0[1] == '4') {
			memset(recvBuf0, 0, sizeof recvBuf0);
			endflag = 0;
			printf("程序开始终止……\n");
		}
		else {//解析失败后应该让arduino重新发送，这里暂时不写
			memset(recvBuf0, 0, sizeof recvBuf0);//就算解析失败也要清空字符串
			printf("解析失败！\n");
		}
	}

	return;
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

	if (direction[number - 1] != 3 && spot[a][b - 1] != 1) {//如果上一步没有向下走，而且上面没有障碍物，那么这步就可以向上走（否则就返回了）
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
	if (direction[number - 1] != 4 && spot[a + 1][b] != 1) {//如果上一步没有向左走，而且右面没有障碍物，那么这步就可以向右走（否则就返回了）
		steps++;//步数+1
		if (direction[number - 1] != 2)turns++;//如果上一步没有向右走，那么就转了一次弯
		a += 1;//走了一步
		direction[number] = 2;
		number += 1;
		shitan();
		number -= 1;
		a -= 1;
		if (direction[number - 1] != 2)turns--;
		steps--;//如果搜到底了，递归回来
	}
	if (direction[number - 1] != 1 && spot[a][b + 1] != 1) {//如果上一步没有向上走，而且下面没有障碍物，那么这步就可以向下走（否则就返回了）
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
	if (direction[number - 1] != 2 && spot[a - 1][b] != 1) {//如果上一步没有向右走，而且左面没有障碍物，那么这步就可以向左走（否则就返回了）
		steps++;//步数+1
		if (direction[number - 1] != 4)turns++;//如果上一步没有向左走，那么就转了一次弯
		a -= 1;//走了一步
		direction[number] = 4;
		number += 1;
		shitan();
		number -= 1;
		a += 1;
		if (direction[number - 1] != 4)turns--;
		steps--;//如果搜到底了，递归回来
	}
}

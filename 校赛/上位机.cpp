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

/*全局函数与变量声明✧(≖ ◡ ≖✿ */
DWORD dwLength;//输入缓冲区的大小（字节数）
char recvBuf[10];//输入的信息保存在该字符串中
DWORD dwactlen;//输出缓冲区的大小（字节数）
char psendbuf[10];//输出的信息保存在该字符串中
DCB myDCB;//初始化串口配置信息DCB结构
HANDLE m_hComm;//串口句柄，定义串口变量

bool COM3();//COM3接口初始化函数
void tance();//接收来自arduino的命令并执行

pthread_t t1;                           //pthread_t变量t1，用于存储线程ID
int drink;
bool endflag = 1;

void* My_thread(void* args) {            //子线程函数
	//clock_t start = clock();
	system("python E:\\lalala\\classify_image_inception_v3.py");
	//clock_t end = (clock() - start) / CLOCKS_PER_SEC;//计时器
	//cout << "time comsumption is " << end << endl;
	return NULL;
}

//-------(￣▽￣)／程序开始---------
int main() {
	clock_t start = clock();
	if (COM3() == FALSE) return 0;//COM3接口初始化

	while (endflag == 1)tance();

	CloseHandle(m_hComm); //m_hComm是CreateFile函数返回的串口句柄，关闭串口，程序结束
	printf("串口关闭\n");
	clock_t end = (clock() - start) / CLOCKS_PER_SEC;//计时器
	cout << "time comsumption is " << end << endl;
	return 0;
}

bool COM3() {
loop:m_hComm = CreateFile(//打开端口
	"COM3:",//使用COM3端口
	GENERIC_READ | GENERIC_WRITE,  //允许读和写
	0,                           //独占方式（共享模式）此处必须独占
	NULL,
	OPEN_EXISTING,              //打开而不是创建（创建方式）
	0,
	NULL
);
	 if (m_hComm == (HANDLE)-1) {                          //打开串口失败返回
		 printf("打开串口失败--->请检查USB是否正确连接！！！\n尝试重连中……\n");
		 Sleep(1000);//延时1s
		 goto loop;//返回尝试再次打开串口
		 return FALSE;
	 }
	 printf("打开串口成功！\n");
	 //得到打开串口的当前属性参数，修改后再重新设置串口。


	 if (!GetCommState(m_hComm, &myDCB)) {
		 printf("无法获取串口当前属性\n");
		 return FALSE;
	 }


	 //设置串口参数  (这段话没弄懂前不要修改，除了波特率）
	 myDCB.BaudRate = CBR_9600;   // 设置波特率9600
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
		 printf("设置串口失败\n");
		 return FALSE;
	 }

	 SetupComm(m_hComm, 1024, 1024);//设置串口的输入/输出缓冲区大小
	 PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);//清空缓冲区，作为初始化

													   //超时的作用是在指定的时间内没有读入或发送指定数量的字符，ReadFile或WriteFile的操作仍然会结束。
													   //超时设置
	 COMMTIMEOUTS TimeOuts;
	 //设定读超时
	 TimeOuts.ReadIntervalTimeout = 100;//读间隔超时(指在接收时两个字符之间的最大时延)
	 TimeOuts.ReadTotalTimeoutMultiplier = 100;//读时间系数
	 TimeOuts.ReadTotalTimeoutConstant = 1000000;//读时间常量
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

void tance() {
	printf("读取中……\n");

	ReadFile(m_hComm, recvBuf, 40, &dwLength, NULL);
	printf("收到数据：%s\n", recvBuf);//输出收到的信息
	PurgeComm(m_hComm, PURGE_RXCLEAR);//每次接收信息之后清空缓存区

									  //开始解析收到的数据内容
	if (recvBuf[1] == '2') {
		memset(recvBuf, 0, sizeof recvBuf);
		printf("开始识别……\n");
		pthread_create(&t1, NULL, My_thread, NULL);  //创建缺省线程
	}
	else if (recvBuf[1] == '3') {
		memset(recvBuf, 0, sizeof recvBuf);
		FILE *fp;//开始读写的句柄
		fopen_s(&fp, "predict_result.txt", "r+");
		fscanf_s(fp, "%d", &drink);
		if (drink == 1)printf_s("AD钙奶\n");
		else if (drink == 2)printf_s("红牛\n");
		else if (drink == 3)printf_s("雪花啤酒\n");
		fclose(fp);

		//回复arduino
		psendbuf[0] = '0' + drink;
		printf_s("向arduino发送：%s\n", psendbuf);
		WriteFile(m_hComm, psendbuf, 10, &dwactlen, NULL);
		PurgeComm(m_hComm, PURGE_TXCLEAR);//每次发完指令都要清空输出寄存器
		memset(psendbuf, 0, sizeof psendbuf);//每次发完指令也要清空保存指令的字符串

										  //清空文件内容
		fopen_s(&fp, "predict_result.txt", "w+");
		fclose(fp);//open和close要相随，关闭文件
	}
	else if (recvBuf[1] == '4') {
		memset(recvBuf, 0, sizeof recvBuf);
		endflag = 0;
		printf("程序开始终止……\n");
	}
	else {//解析失败后应该让arduino重新发送，这里暂时不写
		memset(recvBuf, 0, sizeof recvBuf);//就算解析失败也要清空字符串
		printf("解析失败！\n");
	}

	return;
}

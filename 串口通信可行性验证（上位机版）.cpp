//串口通信可行性验证（上位机版） 1.2
/*实现功能：当下位机发现指南针传感器指向北方（±2°）时，向上位机发送指令#00！（只发送一次），上位机收到指令后向机械臂发送指令，使之运动。
1.添加部分注释
2.修改程序逻辑BUG
3.串口通信可行性验证通过
*/
//by czl
//2019/02/27/13:16

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <winnt.h>
#include <iostream>
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
void ReadingUNO();//读取下位机发送的指令
int explain();//下位机指令解析函数

			  //其他杂项函数
void Close();//关闭串口等程序关闭前的准备工作
			 //-------(￣▽￣)／程序开始---------
int main() {
	//串口准备
	if (COM3() == FALSE) return 0;//COM3（机械臂）接口初始化
	hello();//接收打招呼信息，准备完毕

	if (COM5() == FALSE) return 0;//COM5（下位机）接口初始化

								  //开始工作
	ReadingUNO();//开始接收arduino发出的指令
	if (explain() == 0) {
		//开始对机械臂发出指令
		strcpy_s(psendbuf, "G0000#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!"); arm(psendbuf);//回到初始状态
		strcpy_s(psendbuf, "G0001#000P1700T1000!#001P1700T1000!#002P1700T1000!#003P1700T1000!#004P1700T1000!#005P1500T1000!"); arm(psendbuf);
		strcpy_s(psendbuf, "G0002#000P1300T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!"); arm(psendbuf);
		strcpy_s(psendbuf, "G0003#000P1500T1000!#001P1500T1000!#002P1500T1000!#003P1500T1000!#004P1500T1000!#005P1500T1000!"); arm(psendbuf);
	}
	else if (explain() == 1) {
		return 0;
	}

	Close();
	return 0;
}

bool COM3() {
loop:m_hComm = CreateFile(//打开端口
	"COM3:",//机械臂使用COM3端口
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
	"COM5:",//下位机使用COM5端口
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

void ReadingUNO() {
	recvBuf0[0] = 'N';//把第一位作为识别位，用来判断是否读入指令
	printf("下位机指令读取中……\n");
	while (recvBuf0[0] == 'N') {//如果没有读到下位机发送的指令，则不断重复读取
		ReadFile(m_hComm0, recvBuf0, 4, &dwLength0, NULL);//读入下位机指令
		PurgeComm(m_hComm, PURGE_RXCLEAR);//清空输入缓冲区
	}
	printf_s("收到下位机发送的指令：%s\n", recvBuf0);
}

int explain() {//指令格式为#XX！（X可为0-9和a-Z）
	if (recvBuf0[0] == '#'&&recvBuf0[3] == '!')
		if (recvBuf0[1] == '0'&&recvBuf0[2] == '0') {
			printf("启动机械臂！\n");
			return 0;
		}
		else printf("下位机命令无效\n");
	else printf("下位机命令格式非法\n");
	return 1;
}

void Close() {
	printf("机械臂指令发送完毕！\n");
	CloseHandle(m_hComm); //m_hComm是CreateFile函数返回的串口句柄，关闭串口，程序结束
	printf("机械臂串口关闭\n");

	CloseHandle(m_hComm0); //关闭串口，程序结束
	printf("下位机串口关闭\n");
}

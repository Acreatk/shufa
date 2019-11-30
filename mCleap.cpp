#include "stdafx.h"
#include "mCleap.h"
#include<Windows.h>
#include<iostream>
#include<fstream>
#include<conio.h>

#include<math.h>
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include "SerialPort.h" 
#include <tchar.h>
#include "internet.h"

#include "leapmotionDlg.h"
#include "Resource.h"

#define		H	    78.0		//
#define		L1 		92.0		//
#define		L2 		87.0		//
#define  	L3		139.0		//
#define 	L1_2 	8464.0
#define		L2_2	7569.0		
#define		L1L2	8007.0                                              
#define		PI 		3.1415926
#define BUFFER_SIZE   64  

int pwm[6] = { 1500,1500,1500,1500,1500,1500 };
bool run_flag = false;

CSerialPort m_serialPort;

ofstream l_log("leap_log.txt");

char Serial_Status = 0;//0:avaliable    1:unavaliable
char DataHex1[22] = {
	0xAA,/*-----------------------SYNC -----0*/
	0xAA,/*-----------------------SYNC -----1*/

	0x12,/*---------------------Length -----2*/

	0xB1,/*---------------------Steer1 -----3*/
	0x0F,/*---------------------Data1 ------4*/
	0x00,/*---------------------Data2 ------5*/

	0xB2,/*---------------------Steer2 -----6*/
	0x0F,/*---------------------Data1 ------7*/
	0x00,/*---------------------Data2 ------8*/

	0xB3,/*---------------------Steer3 -----9*/
	0x0F,/*---------------------Data1 -----10*/
	0x00,/*---------------------Data2 -----11*/

	0xB4,/*---------------------Steer4 ----12*/
	0x0F,/*---------------------Data1 -----13*/
	0x00,/*---------------------Data2 -----14*/

	0xB5,/*---------------------Steer5 ----15*/
	0x0F,/*---------------------Data1 -----16*/
	0x00,/*---------------------Data2 -----17*/

	0xB6,/*---------------------Steer6 ----18*/
	0x0F,/*---------------------Data1 -----19*/
	0x00,/*---------------------Data2 -----20*/

	0x00 /*---------------------Check -----21*/
};
void DataToHex(int data, uint8_t flag)
{
	uint8_t buffer;
	buffer = (data % 100);              //低位
	DataHex1[flag] = (uint8_t)buffer;
	buffer = (data / 100 % 100);        //高位
	DataHex1[flag + 1] = (uint8_t)buffer;
}
void send_com(int pwm[6], char mode) //打包发送给串口,mode != 1->test
{
	char data_buf[16] = { 0 };
	int check = 0;
	char i;
	data_buf[0] = 0x55;
	if (mode == 1) 
	{
		DataToHex(pwm[0], 4);
		DataToHex(pwm[1], 7);
		DataToHex(pwm[2], 10);
		DataToHex(pwm[3], 13);
		DataToHex(pwm[4], 16);
		DataToHex(pwm[5], 19);
		//DataHex[22] = ViceControl;
		int Check;
		Check = DataHex1[20] + DataHex1[19]
			+ DataHex1[18] + DataHex1[17] + DataHex1[16] + DataHex1[15] + DataHex1[14]
			+ DataHex1[13] + DataHex1[12] + DataHex1[11] + DataHex1[10] + DataHex1[9]
			+ DataHex1[8] + DataHex1[7] + DataHex1[6] + DataHex1[5] + DataHex1[4] + DataHex1[3];
		Check = ((Check ^ 0xFFFFFF) & 0xFF);
		DataHex1[21] = Check;
		if (m_serialPort.WriteData(DataHex1, 22) == 1)  //传送数据
		{
			//std::cout << "WriteData Success !" << std::endl;
		}
		else
		{
			//std::cout << "WriteData fail !" << std::endl;
		}
	}
	else //测试信号，测试是否与驱动板键连接
	{
		data_buf[15] = 0x5a;
		if (m_serialPort.WriteData(data_buf, 16) == 1)  //传送数据
		{
			//std::cout << "WriteData Success !" << std::endl;
		}
		else
		{
			//std::cout << "WriteData fail !" << std::endl;
		}
	}

}

int local_launch(LPVOID lpParameter)       //本地模式滤波加执行
{
	CleapmotionDlg* steerCon = (CleapmotionDlg*)lpParameter;
	
	static int pwm_fa[6], pwm_fb[6], pwm_fc[6];
	char c = 0;
	for (c = 0; c < 6; c++) 
	{
		pwm_fc[c] = 0.5*pwm[c] + 0.3*pwm_fb[c] + 0.2*pwm_fa[c];
		((CSliderCtrl*)(steerCon->GetDlgItem(IDC_SLID_SERVO1+c)))->SetPos(pwm_fc[c]);
	}
	if (run_flag) 
	{
		send_com(pwm_fc, 1);
		run_flag = false;
	}

	for (c = 0; c < 6; c++) {
		pwm_fb[c] = pwm_fc[c];
		pwm_fa[c] = pwm_fb[c];
	}


	return 0;

}
int client_launch(LPVOID lpParameter) 
{
	CleapmotionDlg* DLG = (CleapmotionDlg*)lpParameter;
	static int pwm_fa[6], pwm_fb[6], pwm_fc[6];
	int AmountSend;
	char c = 0;
	
	if (run_flag) 
	{
		run_flag = false;
		for (c = 0; c < 6; c++)
		{
			pwm_fc[c] = 0.5*pwm[c] + 0.3*pwm_fb[c] + 0.2*pwm_fa[c];
			((CSliderCtrl*)(DLG->GetDlgItem(IDC_SLID_SERVO1 + c)))->SetPos(pwm_fc[c]);
		}
	}

	for (c = 0; c < 6; c++) 
	{
		pwm_fb[c] = pwm_fc[c];
		pwm_fa[c] = pwm_fb[c];
	}

	return 0;
}

void start(LPVOID lpParameter)
{
	CleapmotionDlg* task = (CleapmotionDlg*)lpParameter;
	if (task->m_Local_Status)
	{
		m_serialPort.InitPort(get_COM(), get_baudrate());
		//com_test();//驱动板连接检测	
		Sleep(500);
		CLeap gcleap;
		Sleep(100);
		Controller controller;
		Sleep(100);
		controller.addListener(gcleap);
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
		while (1)
		{
			if (get_StopFlag() != 3) break;
		}
		// Remove the sample listener when done
		controller.removeListener(gcleap);
	}
	if (task->m_Client_Status)
	{
		Sleep(500);
		CLeap gcleap;
		Sleep(100);
		Controller controller;
		Sleep(100);
		controller.addListener(gcleap);
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
		while (1)
		{
			if (get_StopFlag() != 2) break;
		}
		// Remove the sample listener when done
		controller.removeListener(gcleap);
	}
	if (task->m_Server_Status)
	{
	
		while (1)
		{
			if (get_StopFlag() != 1) break;
		}
	}
}

void CLeap::send_format(int seq, int theat, int time)
{
		int i, j;
		static  char cmd[12];// = "#xPxxxxTxxxx";
		sprintf(cmd + 1, "%d", seq);
		sprintf(cmd + 3, "%d", theat);
		sprintf(cmd + 8, "%d", time);
		*(cmd + 0) = '#';
		*(cmd + 2) = 'P';
		*(cmd + 7) = 'T';
		//CString cmd;
		//cmd.Format("#%dP%dT%d", seq, theat, time);
	
		std::cout <<cmd[0]<< cmd[1] << cmd[2] << cmd[3] << cmd[4] << cmd[5] << cmd[6] 
			<< cmd[7] << cmd[8] << cmd[9] << cmd[10] << cmd[11]<< std::endl;
}

void CLeap::arm(float position[])		        //根据手腕位置推算出舵机123的角度	(75.3, 135.5)
{
	float temp, high, len, sq_temp;
	float cos_2, cos_3, tan_2, tan_3;
	temp = pow(position[0], 2) + pow(position[1], 2) + pow(position[2], 2);
	sq_temp = sqrt(temp);
	if (sq_temp <= 6 || sq_temp >= 170)//此处需要先检验已知点是否在能及范围内//若不在，则需投影到对应的球面上
	{
		if (sq_temp <= 6)
		{
			position[0] = position[0] * 6.0 / sq_temp;
			position[1] = position[1] * 6.0 / sq_temp;
			position[2] = position[2] * 6.0 / sq_temp;
		}
		else if (sq_temp >= 170)
		{
			position[0] = position[0] * 170 / sq_temp;
			position[1] = position[1] * 170 / sq_temp;
			position[2] = position[2] * 170 / sq_temp;
		}
		temp = pow(position[0], 2) + pow(position[1], 2) + pow(position[2], 2);
	}
	len = sqrt(pow(position[0], 2) + pow(position[1], 2) + pow(position[2], 2));
	high = position[2];
	cos_3 = (L1_2 + L2_2 - temp) / (2 * L1L2);

	theat[2] = (int)(2000.0*(acos(cos_3) / PI - 0.55)) + 1500;

	cos_2 = (L1_2 + temp - L2_2) / (2 * L1*sqrt(temp));
	tan_2 = high / len;

	theat[1] = (int)(2000.0*((acos(cos_2) + asin(tan_2)) / PI - 0.59)) + 1500;//

	tan_3 = position[0] / position[1];

	//theat[0] = 1500 - (int)(2000 * atan(tan_3) / PI);

	float static p0_last = 0, p0_now = 0;

	p0_now = position[0];
	//if (fabs(p0_now - p0_last) > 50)p0_now -= (p0_now - p0_last)*0.1;//错误抑制

	if ((1500 - p0_now * 9) > 2500) p0_now = 1000 / -9.0;
	if ((1500 - p0_now * 9) <  500) p0_now = 1000 / 9.0;

	theat[0] = (int)(1500);// -p0_now * 9);



	//if (theat[0] > 2500)theat[0] = 2500;
	//if (theat[0] < 500)theat[0] = 500;
	//int static p3 = 0;
	int p3;
	p3 = 225 - (theat[1] + theat[2] - 1000) * 0.09 + position[3];

	if (p3 >= 10)
		theat[3] = (p3 - 10) * 20 + 500;
	else if (p3 <= -10)
		theat[3] = (p3 + 10) * 20 + 500;

    /*
	if (position[3] >= 10)
		theat[3] = (position[3] - 10) * 20 + 1500;
	else if (position[3] <= -10)
		theat[3] = (position[3] + 10) * 20 + 1500;

	*/

	if (position[4] >= 20)
		theat[4] = (position[4] - 20) * 20 + 1500;
	else if (position[4] <= -20)
		theat[4] = (position[4] + 20) * 20 + 1500;

	theat[5] = 2500 - (position[5] - 30) * 20;
}



void CLeap::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}

void CLeap::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void CLeap::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void CLeap::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}

void CLeap::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	cout << "Leapmotion disconnected" << endl;
	cout << "leap motion intion fialed!/nPress any key exit!" << endl;
	system("pause");
}

void CLeap::onConnect(const Controller &controller)
{
	std::cout << "Leapmotion connected" << endl;
	//打开手势识别
}

void CLeap::onFrame(const Controller &controller)
{
	//实例化一个frame对象用于接收controller.frame()的数据
	const Frame frame = controller.frame();

	if (!frame.hands().isEmpty())
	{
		const Hand hand = frame.hands()[0];  //检测到的第一只手
		Vector armp = hand.arm().elbowPosition();
		const FingerList fingers = hand.fingers(); //手掌的所有手指
		if (!fingers.isEmpty())  //检测一下是否握拳
		{
			Vector Index, Thumb, middle, ring, pinky;
			float x[5], y[6], z[6];
			pinky = fingers[4].tipPosition();
			ring = fingers[3].tipPosition();
			middle = fingers[2].tipPosition();
			Index = fingers[1].tipPosition();
			Thumb = fingers[0].tipPosition();

			//手掌向量的三个夹角
			//pitch = hand.direction().pitch()*RAD_TO_DEG;
			//pitch = Vector::yAxis().angleTo(hand.direction())*RAD_TO_DEG*-1 + 90;
			//roll = Vector::xAxis().angleTo(hand.palmNormal())*RAD_TO_DEG - 90;
			//distance = sqrt((Index.x - Thumb.x)*(Index.x - Thumb.x) + (Index.y - Thumb.y)*(Index.y - Thumb.y) + (Index.z - Thumb.z)*(Index.z - Thumb.z));  //计算距离
			//float x = hand.palmPosition().x*0.3;
			//float z = hand.palmPosition().y*0.3 - 0.5;
			//float y = hand.palmPosition().z*0.3;

			//if (y < 0)
			//{
			//	//	y = -y;
			//}
			////float l = sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2)));
			x[0] = sqrt((Thumb.x - hand.palmPosition().x)*(Thumb.x - hand.palmPosition().x) +
				(Thumb.y - hand.palmPosition().y)*(Thumb.y - hand.palmPosition().y) +
				(Thumb.z - hand.palmPosition().z)*(Thumb.z - hand.palmPosition().z));
			x[1] = sqrt((Index.x - hand.palmPosition().x)*(Index.x - hand.palmPosition().x) +
				(Index.y - hand.palmPosition().y)*(Index.y - hand.palmPosition().y) +
				(Index.z - hand.palmPosition().z)*(Index.z - hand.palmPosition().z));
			x[2] = sqrt((middle.x - hand.palmPosition().x)*(middle.x - hand.palmPosition().x) +
				(middle.y - hand.palmPosition().y)*(middle.y - hand.palmPosition().y) +
				(middle.z - hand.palmPosition().z)*(middle.z - hand.palmPosition().z));
			x[3] = sqrt((ring.x - hand.palmPosition().x)*(ring.x - hand.palmPosition().x) +
				(ring.y - hand.palmPosition().y)*(ring.y - hand.palmPosition().y) +
				(ring.z - hand.palmPosition().z)*(ring.z - hand.palmPosition().z));
			x[4] = sqrt((pinky.x - hand.palmPosition().x)*(pinky.x - hand.palmPosition().x) +
				(pinky.y - hand.palmPosition().y)*(pinky.y - hand.palmPosition().y) +
				(pinky.z - hand.palmPosition().z)*(pinky.z - hand.palmPosition().z));
			//x[5] = Vector::yAxis().angleTo(hand.direction())*RAD_TO_DEG*-1 + 90;
			position[0] = x[0];
			position[1] = x[1];
			position[2] = x[2];
			position[3] = x[3];
			position[4] = x[4];
			//position[5] = x[5];
			
			//arm(position);

			//theat[0] = 1500;
			//theat[1] = 1500;
			//theat[2] = 1500;
			//theat[3] = 1500;
			//theat[4] = 1500;
			//theat[5] = 1500;
			theat[0] = 2500 - (position[0] - 30) * 28;
			theat[1] = (position[1] - 30) * 28;
			theat[2] = (position[2] - 30) * 28;
			theat[3] = (position[3] - 30) * 28;
			theat[4] = 2500 - (position[4] - 30) * 28;
			theat[5] = 2500 - (position[5] - 30) * 25;
			for (int n = 0; n < 6; n++)
			{
				pwm[n] = theat[n];
			}
			run_flag = true;
		}
	}
}

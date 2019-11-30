#pragma once

#include <Leap.h>
using namespace std;
using namespace Leap;

extern int pwm[6];

class CLeap : public Listener
{
public:
	virtual void onConnect(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);

	float   position[6] = { 0.0 };	//
	
	int theat[6] = { 0 };//
	void send_format(int seq, int theat, int time);
	//void go(CSerialPort mySerialPort);
private:
	void arm(float position[]);
	//
	//char model = 1;
};

void start(LPVOID lpParameter);
int local_launch(LPVOID lpParameter);
int client_launch(LPVOID lpParameter);

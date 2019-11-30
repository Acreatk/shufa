
// leapfmotionDlg.h : 头文件
//
#include "DlgClient.h"
#include "DlgLocal.h"
#include "DlgServer.h"
#include "SerialPort.h"
#include "mCleap.h"
#include "sapi.h"
#include <sphelper.h>//导入语音识别头文件
#include <atlstr.h>
#include "DlgConnect.h"
#include "MySocket.h"
#pragma once
#pragma comment(lib,"sapi.lib") //导入语音头文件库

const int WM_RECORD = WM_USER + 100;

int get_baudrate();
int get_COM();
int get_StopFlag();


// CleapfmotionDlg 对话框
class CleapmotionDlg : public CDialogEx
{
// 构造
public:
	CleapmotionDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CleapmotionDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEAPFMOTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLocal();
private:
	DlgServer m_dlgserver;
	DlgClient m_dlgclient;
	DlgLocal  m_dlglocal;
public:
	//bool m_Serial_Status;
	bool m_Local_Status;
	bool m_Client_Status;
	bool m_Server_Status;


	
	void direction_contorl();


public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	afx_msg void OnBnClickedBtnstart();

public:
	afx_msg void OnBnClickedBtnvoice();

	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnVideo();

	friend void VideoProc(LPVOID lpParameter);

	
	afx_msg void OnBnClickedClient();
	afx_msg void OnBnClickedServer();

	
	bool InitSliderData();

	CBrush m_brush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CBitmapButton m_Btn_up;
	CBitmapButton m_Btn_down;
	CBitmapButton m_Btn_left;
	CBitmapButton m_Btn_right;

	CBitmapButton m_Btn_Video;
	CBitmapButton m_Btn_Voice;
	CBitmapButton m_Btn_Connect;

	CListBox m_MSG;

	afx_msg void OnReleasedcaptureSlidServo1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSlidServo2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSlidServo3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSlidServo4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSlidServo5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSlidServo6(NMHDR *pNMHDR, LRESULT *pResult);

	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool Connect_flag;
	bool Vioce_flag;

	int TryCount;
	MySocket m_clientSocket;
	//short m_szPort;
	string  m_szPort;
	string m_szServerAdr;
	//CHAR m_szServerAdr[];
	UINT Timer1, Timer2,Timer3;

	static void DataToHex(int data,uint8_t flag);
	
};

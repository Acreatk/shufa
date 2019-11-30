
// leapfmotionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <strsafe.h>
#include "leapmotion.h"
#include "leapmotionDlg.h"
#include "afxdialogex.h"
#include "Uxtheme.h"
#include "resource.h"
#include "MatOrImage.h"
#include "SerialPort.h" 
#include <io.h>  
#include <fcntl.h>  

#pragma comment(lib, "Uxtheme.lib")//链接“Uxtheme”库
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER1  1
#define TIMER2  2
#define TIMER3  3	//定义了三个定时器
#define WM_RECOEVENT   520

UINT COM = 0;
int BAUDRATE = 0;
uint16_t StopFlag = 0;//typedef unsigned short int

bool CloseFlag = false;
bool vioce_start = false;
uint8_t ViceControl = 0x00;//typedef unsigned char
SOCKET sock;
char DataHex[22] ={
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
uint8_t directionbuf[6]={
	0xAA,/*----------------------0--*/
	0xAA,/*----------------------1--*/
	0x02,/*----------------------2--*/
	0xDD,/*----------------------3--*/
	0x00,/*----------------------4--*/
	0x00 /*----------------------5--*/
};
CComPtr<ISpRecognizer>m_cpRecoEngine;// 语音识别引擎(recognition)的接口。
CComPtr<ISpRecoContext>m_cpRecoCtxt;// 识别引擎上下文(context)的接口。
CComPtr<ISpRecoGrammar>m_cpCmdGrammar;// 识别文法(grammar)的接口。
CComPtr<ISpStream>m_cpInputStream;// 流()的接口。
CComPtr<ISpObjectToken>m_cpToken;// 语音特征的(token)接口。
CComPtr<ISpAudio>m_cpAudio;// 音频(Audio)的接口。(用来保存原来默认的输入流)
ULONGLONG  ullGrammerID;
HWND  m_hWnd;
bool  m_bGotReco;//语音识别相关
//void InitConsole()
//{
//	int nRet = 0;
//	FILE* fp;
//	AllocConsole();
//	nRet = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
//	fp = _fdopen(nRet, "w");
//	*stdout = *fp;
//	setvbuf(stdout, NULL, _IONBF, 0);
//}

int get_COM()
{
	return COM;
}


int get_baudrate()
{
	return BAUDRATE;
}

int get_StopFlag()
{
	return StopFlag;
}

// CleapfmotionDlg 对话框
struct Serialport
{
	CString m_COM;
	int m_BAUD_RATE;
	int m_DATA_BIT;
	int m_STOP_BIT;
	CString m_CHECK_BIT;
};

Serialport m_Port;
HANDLE hThread;
DWORD ThreadID;
DWORD ThreadID1;

CleapmotionDlg::CleapmotionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LEAPFMOTION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_Local_Status = false;
	m_Client_Status = false;
	m_Server_Status = false;
	//InitConsole();
}

CleapmotionDlg::~CleapmotionDlg()
{
	m_cpRecoEngine.Release();//释放语音引擎
	m_cpRecoCtxt.Release();
	m_cpCmdGrammar.Release();
	m_cpAudio.Release();
}

void CleapmotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_UP, m_Btn_up);
	DDX_Control(pDX, IDC_BTN_DOWN, m_Btn_down);
	DDX_Control(pDX, IDC_BTN_LEFT, m_Btn_left);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_Btn_right);
	DDX_Control(pDX, IDC_BTN_VIDEO, m_Btn_Video);
	DDX_Control(pDX, IDC_BTN_VOICE, m_Btn_Voice);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_Btn_Connect);

	DDX_Control(pDX, IDC_LIST1, m_MSG);
}

BEGIN_MESSAGE_MAP(CleapmotionDlg, CDialogEx)//消息映射
	ON_WM_SYSCOMMAND()//截获控制命令
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNSTART, &CleapmotionDlg::OnBnClickedBtnstart)//用户单击按钮时,向父对象发送消息
	ON_BN_CLICKED(IDC_BTN_VOICE, &CleapmotionDlg::OnBnClickedBtnvoice)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CleapmotionDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_VIDEO, &CleapmotionDlg::OnBnClickedBtnVideo)
	ON_BN_CLICKED(IDC_BUTTON3, &CleapmotionDlg::OnBnClickedLocal)
	ON_BN_CLICKED(IDC_BUTTON2, &CleapmotionDlg::OnBnClickedClient)
	ON_BN_CLICKED(IDC_BUTTON1, &CleapmotionDlg::OnBnClickedServer)
	ON_WM_CTLCOLOR()
	//舵机
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_SERVO1, &CleapmotionDlg::OnReleasedcaptureSlidServo1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_SERVO2, &CleapmotionDlg::OnReleasedcaptureSlidServo2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_SERVO3, &CleapmotionDlg::OnReleasedcaptureSlidServo3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_SERVO4, &CleapmotionDlg::OnReleasedcaptureSlidServo4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_SERVO5, &CleapmotionDlg::OnReleasedcaptureSlidServo5)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_SERVO6, &CleapmotionDlg::OnReleasedcaptureSlidServo6)

END_MESSAGE_MAP()


// CleapfmotionDlg 消息处理程序

BOOL CleapmotionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);//返回当前系统菜单句柄
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);//添加一条水平线
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);//IDM_ABOUTBOX表示要执行的动作
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	Timer1 = SetTimer(TIMER1, 200, NULL);//启动定时器1,定时时间是100ms

	InitSliderData();

	m_brush.CreateSolidBrush(RGB(45, 45, 48));
	SetWindowTheme(GetDlgItem(IDC_GROUP1)->GetSafeHwnd(), L"", L"");

	m_Btn_down.LoadBitmaps(IDB_BITMAP1, IDB_BITMAP2);//载入 
	m_Btn_down.SizeToContent();

	m_Btn_left.LoadBitmaps(IDB_BITMAP3, IDB_BITMAP4);//载入 
	m_Btn_left.SizeToContent();

	m_Btn_right.LoadBitmaps(IDB_BITMAP5, IDB_BITMAP6);//载入 
	m_Btn_right.SizeToContent();

	m_Btn_up.LoadBitmaps(IDB_BITMAP7, IDB_BITMAP8);//载入 
	m_Btn_up.SizeToContent();

	m_Btn_Video.LoadBitmaps(IDB_BITMAPVIDEO);//载入 
	m_Btn_Video.SizeToContent();

	m_Btn_Voice.LoadBitmaps(IDB_BITMAPVICE, IDB_BITMAPVICE1);//载入 
	m_Btn_Voice.SizeToContent();

	m_Btn_Connect.LoadBitmaps(IDB_BITMAPCON);//载入 
	m_Btn_Connect.SizeToContent();

	//使方向按键禁能
	((CButton*)GetDlgItem(IDC_BTN_UP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_DOWN))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_LEFT))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BTN_RIGHT))->EnableWindow(false);

	//((CButton*)GetDlgItem(IDC_BTN_VIDEO))->EnableWindow(false);

	Connect_flag = false;
	Vioce_flag = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CleapmotionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CleapmotionDlg::OnPaint()
{
	if (IsIconic())//窗口是否最小化
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		//CRect rect;
		//CPaintDC dc(this);
		//GetClientRect(rect);
		//dc.FillSolidRect(rect, RGB(240, 255, 255));//设置为绿色背景
	}
	else
	{
		//CRect rect;
		//CPaintDC dc(this);
		//GetClientRect(rect);
		//dc.FillSolidRect(rect, RGB(150, 255, 255));//设置为绿色背景
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CleapmotionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CleapmotionDlg::OnBnClickedBtnstart()//程序开始按键
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_MSG.AddString(L"Started!");
	if (m_Local_Status)
	{
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start, (LPVOID)this, 0, &ThreadID);
	}
	else if (m_Client_Status)
	{
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start, (LPVOID)this, 0, &ThreadID);
	}
	else if (m_Server_Status)
	{
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start, (LPVOID)this, 0, &ThreadID);
	}
	else
	{
		m_MSG.AddString(L"Please select mode!");
	}
	UpdateData(false);
}

#pragma region Speakcode
//文字转语音
void  MSSSpeak(LPCTSTR speakContent)
{
	ISpVoice *pVoice = NULL;

	//初始化COM接口

	if (FAILED(::CoInitialize(NULL)))
		MessageBox(NULL, (LPCWSTR)L"COM接口初始化失败！",
		(LPCWSTR)"提示", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);

	//获取SpVoice接口

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);


	if (SUCCEEDED(hr))
	{
		pVoice->SetVolume((USHORT)100); //设置音量，范围是 0 -100
		pVoice->SetRate(2); //设置速度，范围是 -10 - 10
		hr = pVoice->Speak(speakContent, 0, NULL);

		pVoice->Release();

		pVoice = NULL;
	}

	//释放com资源
	::CoUninitialize();
}


//语音转文字
void MSSListen()
{

	//初始化COM接口

	if (FAILED(::CoInitialize(NULL)))
		MessageBox(NULL, (LPCWSTR)L"COM接口初始化失败！",
		(LPCWSTR)L"提示", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);



	HRESULT hr = m_cpRecoEngine.CoCreateInstance(CLSID_SpSharedRecognizer);//创建Share型识别引擎
	if (SUCCEEDED(hr))
	{


		hr = m_cpRecoEngine->CreateRecoContext(&m_cpRecoCtxt);//创建识别上下文接口

		hr = m_cpRecoCtxt->SetNotifyWindowMessage(m_hWnd, WM_RECORD, 0, 0);//设置识别消息

		const ULONGLONG ullInterest = SPFEI(SPEI_SOUND_START) | SPFEI(SPEI_SOUND_END) | SPFEI(SPEI_RECOGNITION);//设置我们感兴趣的事件
		hr = m_cpRecoCtxt->SetInterest(ullInterest, ullInterest);

		hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &m_cpAudio);
		m_cpRecoEngine->SetInput(m_cpAudio, true);



		//创建语法规则
		//dictation听说式
		//hr = m_cpRecoCtxt->CreateGrammar(GIDDICTATION, &m_cpDictationGrammar);
		//if (SUCCEEDED(hr))
		//{
		//	hr = m_cpDictationGrammar->LoadDictation(NULL, SPLO_STATIC);//加载词典
		//}

		//C&C命令式，此时语法文件使用xml格式
		ullGrammerID = 1000;
		hr = m_cpRecoCtxt->CreateGrammar(ullGrammerID, &m_cpCmdGrammar);

		WCHAR wszXMLFile[20] = L"";//加载语法
		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)"CmdCtrl.xml", -1, wszXMLFile, 256);//ANSI转UNINCODE
		hr = m_cpCmdGrammar->LoadCmdFromFile(wszXMLFile, SPLO_DYNAMIC);


		//MessageBox(NULL, (LPCWSTR)L"语音识别已启动！", (LPCWSTR)L"提示", MB_CANCELTRYCONTINUE );
		//激活语法进行识别
		//hr = m_cpDictationGrammar->SetDictationState(SPRS_ACTIVE);//dictation
		hr = m_cpCmdGrammar->SetRuleState(NULL, NULL, SPRS_ACTIVE);//C&C
		hr = m_cpRecoEngine->SetRecoState(SPRST_ACTIVE);

	}

	else
	{
		//MessageBox(L"语音识别引擎启动出错！");
		MessageBox(NULL, (LPCWSTR)L"语音识别引擎启动出错！", (LPCWSTR)L"警告", MB_OK);
		exit(0);
	}


	//释放com资源
	::CoUninitialize();
	//hr = m_cpCmdGrammar->SetRuleState(NULL, NULL, SPRS_INACTIVE);//C&C


}

void VOICE_start(LPVOID lpParameter)
{
	USES_CONVERSION;
	CSpEvent event;
	CleapmotionDlg* viocetask = (CleapmotionDlg*)lpParameter;
	while (1)
		if (m_cpRecoCtxt)
		{
			while (event.GetFrom(m_cpRecoCtxt) == S_OK)
			{

				switch (event.eEventId)
				{
				case SPEI_RECOGNITION:
				{
					//识别出了语音
					m_bGotReco = TRUE;

					static const WCHAR wszUnrecognized[] = L"<Unrecognized>";

					CSpDynamicString dstrText;

					////取得识别结果 
					if (FAILED(event.RecoResult()->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL)))
					{
						dstrText = wszUnrecognized;
					}

					BSTR SRout;
					dstrText.CopyToBSTR(&SRout);
					CString Recstring;
					Recstring.Empty();
					Recstring = SRout;

					//方向
					if (Recstring == L"前进" || Recstring == L"前")
					{
						ViceControl = 0x80;
						viocetask->Vioce_flag = true;
						//MSSSpeak(LPCTSTR(_T("前进")));
					}
					else if (Recstring == L"左转" || Recstring == L"左")
					{
						ViceControl = 0x40;
						viocetask->Vioce_flag = true;
						//MSSSpeak(LPCTSTR(_T("向左")));
					}
					else if (Recstring == L"右转" || Recstring == L"右")
					{
						ViceControl = 0x20;
						viocetask->Vioce_flag = true;
						//MSSSpeak(LPCTSTR(_T("向右")));
					}
					else if (Recstring == L"后退" || Recstring == L"后")
					{
						ViceControl = 0x10;
						viocetask->Vioce_flag = true;
						//MSSSpeak(LPCTSTR(_T("后退")));
					}

					//速度
					else if (Recstring == L"快速前进")
					{
						ViceControl = 0x88;
						viocetask->Vioce_flag = true;
						//MSSSpeak(LPCTSTR(_T("快速")));
					}
					else if (Recstring == L"中速前进")
					{
						ViceControl = 0x84;
						viocetask->Vioce_flag = true;
						//MSSSpeak(LPCTSTR(_T("中速")));
					}
					else if (Recstring == L"慢速前进")
					{
						ViceControl = 0x82;
						viocetask->Vioce_flag = true;
						//MSSSpeak(LPCTSTR(_T("慢速")));
					}
					else if (Recstring == L"停止")
					{
						ViceControl = 0x00;
						viocetask->Vioce_flag = true;
						//MSSSpeak(LPCTSTR(_T("停止")));
					}
					else if (Recstring == L"语音控制")
					{
						ViceControl = 0x00;
						viocetask->Vioce_flag = true;
						vioce_start = true;
						//MSSSpeak(LPCTSTR(_T("停止")));
					}
				}
				break;
				}
			}
		}
}

void CleapmotionDlg::OnBnClickedBtnvoice()
{
	// TODO: 在此添加控件通知处理程序代码
	// Init MSS
	MSSListen();
	//MSSSpeak(LPCTSTR(_T("您需要帮助吗 ")));
	UpdateData();
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VOICE_start, (LPVOID)this, 0, &ThreadID1);
	UpdateData(false);

}
//
#pragma endregion



void CleapmotionDlg::OnBnClickedBtnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	m_clientSocket.ShutDown(2);
	m_clientSocket.m_hSocket = INVALID_SOCKET;
	m_clientSocket.m_bConnected = FALSE;

	CDlgConnect m_Dlg;
	if (m_Dlg.DoModal() == IDOK && !m_Dlg.m_Addr.IsEmpty())
	{
		m_szServerAdr =  CW2A(m_Dlg.m_Addr.GetBuffer());
		m_szPort = CW2A(m_Dlg.m_Port.GetBuffer()); //建立计时器，每1秒尝试连接一次，直到连上或TryCount>10
		Timer2 = SetTimer(TIMER2, 1000, NULL);
		TryCount = 0;
	}

	((CButton*)GetDlgItem(IDC_BTN_UP))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_DOWN))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_LEFT))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_RIGHT))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BTN_VIDEO))->EnableWindow(true);

	Connect_flag = true;
	//connect(m_socket, (SOCKADDR*)&addrTo, sizeof(SOCKADDR));

}
VideoCapture video;
#pragma region video_code
void VideoProc(LPVOID lpParameter)
{
	CleapmotionDlg* videotask = (CleapmotionDlg*)lpParameter;
	MatOrImage imagechange;
	Mat src;
	ATL::CImage m_image;

	const string address = "http://192.168.0.118:8080/?action=stream";

	if (video.open(address))
	{
		while (true)
		{

			video >> src;
			imagechange.MatToCImage(src, m_image);
			
			CRect rect, srect;
			int cx = m_image.GetWidth();
			int cy = m_image.GetHeight();

			videotask->GetDlgItem(IDC_VIDEO)->GetWindowRect(&rect);
			/*-------------------------------------------------*/
			//videotask->ScreenToClient(&rect);
			//videotask->GetDlgItem(IDC_ShowImage)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
			/*------------------------------------------------*/
			CWnd *pWnd = NULL;
			pWnd = videotask->GetDlgItem(IDC_VIDEO);//获取控件句柄  
			pWnd->GetClientRect(&rect);//获取句柄指向控件区域的大小
			CDC *pDc = NULL;
			pDc = pWnd->GetDC();//获取picture的DC
			m_image.StretchBlt(pDc->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, cx, cy, SRCCOPY);//拉伸显示
																									 //m_image.Draw(pDc->m_hDC, rect);
			videotask->ReleaseDC(pDc);
			if (CloseFlag)
				break;
		}
	}
}


void CleapmotionDlg::OnBnClickedBtnVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_BTN_VIDEO)->GetWindowText(str);
	if (str == "Video")
	{
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(L"Video is opening! Please wait a moment!");
		CloseFlag = false;
		GetDlgItem(IDC_BTN_VIDEO)->SetWindowText(L"Close");
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VideoProc, (LPVOID)this, 0, NULL);
		CloseHandle(hThread);
	}
	else
	{
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(L"Video is off!");
		GetDlgItem(IDC_BTN_VIDEO)->SetWindowText(L"Video");
		CloseFlag = true;

	}
}
#pragma endregion



void CleapmotionDlg::OnBnClickedServer()//server
{
	// TODO: 在此添加控件通知处理程序代码
	m_Local_Status = false;
	m_Client_Status = false;
	m_Server_Status = true;
	StopFlag = 1;
}

void CleapmotionDlg::OnBnClickedClient()//Client
{
	// TODO: 在此添加控件通知处理程序代码
	m_Local_Status = false;
	m_Client_Status = true;
	m_Server_Status = false;
	StopFlag = 2;
}

void CleapmotionDlg::OnBnClickedLocal()//Local
{
	// TODO: 在此添加控件通知处理程序代码

	int flag;
	flag = m_dlglocal.DoModal();
	if (flag == IDOK)
	{
		CString str;

		m_Port.m_COM = m_dlglocal.m_portname;
		m_Port.m_BAUD_RATE = _ttoi(m_dlglocal.m_baudrate);
		m_Port.m_DATA_BIT = _ttoi(m_dlglocal.m_databit);
		m_Port.m_STOP_BIT = _ttoi(m_dlglocal.m_stopbit);
		m_Port.m_CHECK_BIT = m_dlglocal.m_checkbit;


		str = "Port Name:";
		str += m_Port.m_COM;
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);

		str.Format(L"Baud Bits:%d", m_Port.m_BAUD_RATE);
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);

		str.Format(L"Data Bit:%d", m_Port.m_DATA_BIT);
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);

		str.Format(L"Stop Bit:%d", m_Port.m_STOP_BIT);
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);

		str = "Check Bit:";
		str += m_Port.m_CHECK_BIT;
		((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);

		COM = m_Port.m_COM.GetAt(3) - 48;
		BAUDRATE = m_Port.m_BAUD_RATE;

		m_Local_Status = true;
		m_Client_Status = false;
		m_Server_Status = false;
		StopFlag = 3; 
	}

}
void CleapmotionDlg::direction_contorl()
{
	string directioncontrol;

	if (ViceControl&0x80)//前
		directioncontrol = "direction:run!";

	if (ViceControl&0x40)//左
		directioncontrol = "direction:left!";

	if (ViceControl&0x20)//右
		directioncontrol = "direction:right!";

	if (ViceControl&0x10)//后
		directioncontrol = "direction:back!";

	if (ViceControl&0x01)//停止
		directioncontrol = "direction:stop!";

}

void CleapmotionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case TIMER1:
		UpdateData();
		if (m_Local_Status)
		{
			local_launch(LPVOID(this));

		}
		if (m_Client_Status)
		{
			client_launch(LPVOID(this));
			CString str;
			int pos[6];
			//int pos;
			Sleep(20);
			for (int i = 0; i < 6; i++)
			{
				pos[i] = ((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO1 + i))->GetPos();
			}
			DataToHex(pos[0], 4);
			DataToHex(pos[1], 7);
			DataToHex(pos[2], 10);
			DataToHex(pos[3], 13);
			DataToHex(pos[4], 16);
			DataToHex(pos[5], 19);
			//DataHex[22] = ViceControl;
			int Check;
			Check = DataHex[20] + DataHex[19]
				+ DataHex[18] + DataHex[17] + DataHex[16] + DataHex[15] + DataHex[14]
				+ DataHex[13] + DataHex[12] + DataHex[11] + DataHex[10] + DataHex[9]
				+ DataHex[8] + DataHex[7] + DataHex[6] + DataHex[5] + DataHex[4] + DataHex[3];
			Check = ((Check ^ 0xFFFFFF) & 0xFF);
			DataHex[21] = Check;
			if (m_clientSocket.m_bConnected)
			{
				send(sock, DataHex, 22, 0);
			}
		}
		if (m_Server_Status)
		{

		}
		if (Vioce_flag&&vioce_start)
		{
			Vioce_flag = false;
			if (m_clientSocket.m_bConnected)
			{
				uint8_t check;
				directionbuf[4] = ViceControl;
				check = directionbuf[3] + directionbuf[4];
				check = ~check;
				directionbuf[5] = check;
				send(sock, (char *)directionbuf,6, 0);
				/*memcpy(m_clientSocket.m_szBuffer, directionbuf, 6);
				m_clientSocket.m_nLength = 6;
				m_clientSocket.AsyncSelect(FD_WRITE);*/
			}
		}
		break;
	case TIMER2:

		if (m_clientSocket.m_hSocket == INVALID_SOCKET)
		{
			//SOCKET client_sock;  //客户端本地套接字
			struct addrinfo server_addr, *addrinfo = NULL; //服务器端地址
			memset(&server_addr, 0, sizeof(server_addr));//在getaddrinfo()之前必须清0
			server_addr.ai_family = AF_INET6;  //指定是ipv6协议
			server_addr.ai_socktype = SOCK_STREAM; //流式
			server_addr.ai_protocol = IPPROTO_TCP;  //TCP协议,也可以是0，让系统自己选
		
			
			//delete addr_data;
			//PCSTR m_serverPORT = (PCSTR)(PCTSTR)(m_szPort);
			int result = getaddrinfo("2001:da8:c000:2021:a3e2:35c0:3bcf:8900", "10008", &server_addr, &addrinfo); 
		    //int result = getaddrinfo("fd51::42f8:adde:d32e:370f", "10008", &server_addr, &addrinfo); //服务器端ipv6地址, 服务器监听的端口
			//int result = getaddrinfo(addr_data, "10008", &server_addr, &addrinfo);  //服务器端ipv6地址, 服务器监听的端口

			if (result != 0)//地址解析正确时返回0
			{
				m_MSG.AddString(L"getaddrinfo error!\n");
				//cout << "客户端解析服务器地址失败!" << endl;
				return;
			}
			sock = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol); //创建客户端套接字
			if (sock == INVALID_SOCKET)
			{
				m_MSG.AddString(L"create socket error!\n");
				//cout << "创建客户端socket失败!" << endl;
				return;
			}
			if (connect(sock, addrinfo->ai_addr, addrinfo->ai_addrlen) == SOCKET_ERROR)  //连接服务器
			{
				
				m_MSG.AddString(L"connected error!\n");
				//cout << "连接服务器出错!" << endl;
				return;
			}
			else {
				m_clientSocket.m_bConnected = true;
				m_MSG.AddString(L"CONNNECTED!!!!\n");
				KillTimer(TIMER2);
			}
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


bool CleapmotionDlg::InitSliderData()
{
	//Servo1
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO1))->SetRange(500, 2500, true);
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO1))->SetPos(1500);
	//Servo2
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO2))->SetRange(500, 2500, true);
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO2))->SetPos(1500);
	//Servo3
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO3))->SetRange(500, 2500, true);
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO3))->SetPos(1500);
	//Servo4
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO4))->SetRange(500, 2500, true);
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO4))->SetPos(1500);
	//Servo5
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO5))->SetRange(500, 2500, true);
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO5))->SetPos(1500);
	//Servo6
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO6))->SetRange(500, 2500, true);
	((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO6))->SetPos(1500);
	return true;
}


HBRUSH CleapmotionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}

	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetTextColor(RGB(0,255, 0));//文本黑色
		//pDC->SetBkColor(RGB(0, 255, 100));
		//hbr = (HBRUSH)m_brush;
	}
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0, 0, 0));//文本黑色
		//pDC->SetBkColor(RGB(150, 255, 255));
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)m_brush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_GROUP1)
	{
		//pDC->SetTextColor(RGB(0, 0, 0));//文本黑色
		//pDC->SetBkMode(TRANSPARENT);
		//CRect rect;
		//CWnd *wnd;
		//wnd = GetDlgItem(IDC_GROUP1);
		//wnd->GetClientRect(&rect);
		//CDC *pDc = NULL;
		//pDc = wnd->GetDC();//获取picture的DC
		//pDc->FillSolidRect(&rect, RGB(255, 255, 255));
		//hbr = (HBRUSH)m_brush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CleapmotionDlg::OnReleasedcaptureSlidServo1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	int data = ((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO1))->GetPos();
	str.Format(L"Servo1:%d", data);
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);
	*pResult = 0;
}


void CleapmotionDlg::OnReleasedcaptureSlidServo2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	int data = ((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO2))->GetPos();
	str.Format(L"Servo2:%d", data);
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);
	*pResult = 0;
}


void CleapmotionDlg::OnReleasedcaptureSlidServo3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	int data = ((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO3))->GetPos();
	str.Format(L"Servo3:%d", data);
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);
	*pResult = 0;
}


void CleapmotionDlg::OnReleasedcaptureSlidServo4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	int data = ((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO4))->GetPos();
	str.Format(L"Servo4:%d", data);
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);
	*pResult = 0;
}


void CleapmotionDlg::OnReleasedcaptureSlidServo5(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	int data = ((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO5))->GetPos();
	str.Format(L"Servo5:%d", data);
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);
	*pResult = 0;
}


void CleapmotionDlg::OnReleasedcaptureSlidServo6(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	int data = ((CSliderCtrl*)GetDlgItem(IDC_SLID_SERVO6))->GetPos();
	str.Format(L"Servo6:%d", data);
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(str);
	*pResult = 0;
}



BOOL CleapmotionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	if (pMsg->message == WM_LBUTTONDOWN)
	{	
		
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_UP)->m_hWnd)
		{
			// 在此调用Button按钮按下的操作
			if (m_clientSocket.m_bConnected)
			{
				uint8_t check;
				directionbuf[4] = 0x80;
				check = directionbuf[3] + directionbuf[4];
				check = ~check;
				directionbuf[5] = check;
				send(sock, "Button down!\n",14 , 0);
				m_MSG.AddString(L"Button down!\n");
				/*memcpy(m_clientSocket.m_szBuffer, directionbuf, 6);
				m_clientSocket.m_nLength = 6;
				m_clientSocket.AsyncSelect(FD_WRITE);*/
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_DOWN)->m_hWnd)
		{
			// 在此调用Button按钮按下的操作
			if (m_clientSocket.m_bConnected)
			{
				uint8_t check;
				directionbuf[4] = 0x10;
				check = directionbuf[3] + directionbuf[4];
				check = ~check;
				directionbuf[5] = check;
				send(sock, (char *)directionbuf, 6, 0);
				m_MSG.AddString(L"Button down!\n");
				/*memcpy(m_clientSocket.m_szBuffer, directionbuf, 6);
				m_clientSocket.m_nLength = 6;
				m_clientSocket.AsyncSelect(FD_WRITE);*/
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_LEFT)->m_hWnd)
		{
			// 在此调用Button按钮按下的操作
			if (m_clientSocket.m_bConnected)
			{
				uint8_t check;
				directionbuf[4] = 0x20;
				check = directionbuf[3] + directionbuf[4];
				check = ~check;
				directionbuf[5] = check;
				send(sock, (char *)directionbuf, 6, 0);
				m_MSG.AddString(L"Button down!\n");
			/*	memcpy(m_clientSocket.m_szBuffer, directionbuf, 6);
				m_clientSocket.m_nLength = 6;
				m_clientSocket.AsyncSelect(FD_WRITE);*/
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_RIGHT)->m_hWnd)
		{
			// 在此调用Button按钮按下的操作
			if (m_clientSocket.m_bConnected)
			{
				uint8_t check;
				directionbuf[4] = 0x40;
				check = directionbuf[3] + directionbuf[4];
				check = ~check;
				directionbuf[5] = check;
				send(sock, (char *)directionbuf, 6, 0);
				m_MSG.AddString(L"Button down!\n");
				/*memcpy(m_clientSocket.m_szBuffer, directionbuf, 6);
				m_clientSocket.m_nLength = 6;
				m_clientSocket.AsyncSelect(FD_WRITE);*/
			}
		}

	}

	if (pMsg->message == WM_LBUTTONUP)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_UP)->m_hWnd)
		{
			// 在此调用Button按钮抬起的操作
			if (m_clientSocket.m_bConnected)
			{
				uint8_t check;
				directionbuf[4] = 0x00;
				check = directionbuf[3] + directionbuf[4];
				check = ~check;
				directionbuf[5] = check;
				send(sock, (char *)directionbuf, 6, 0);
				m_MSG.AddString(L"Button down!\n");
				/*memcpy(m_clientSocket.m_szBuffer, directionbuf, 6);
				m_clientSocket.m_nLength = 6;
				m_clientSocket.AsyncSelect(FD_WRITE);*/
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_DOWN)->m_hWnd)
		{
			// 在此调用Button按钮抬起的操作
			if (m_clientSocket.m_bConnected)
			{
				uint8_t check;
				directionbuf[4] = 0x00;
				check = directionbuf[3] + directionbuf[4];
				check = ~check;
				directionbuf[5] = check;
				send(sock, (char *)directionbuf, 6, 0);
				m_MSG.AddString(L"Button down!\n");
				/*memcpy(m_clientSocket.m_szBuffer, directionbuf, 6);
				m_clientSocket.m_nLength = 6;
				m_clientSocket.AsyncSelect(FD_WRITE);*/
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_LEFT)->m_hWnd)
		{
			// 在此调用Button按钮抬起的操作
			if (m_clientSocket.m_bConnected)
			{
				uint8_t check;
				directionbuf[4] = 0x00;
				check = directionbuf[3] + directionbuf[4];
				check = ~check;
				directionbuf[5] = check;
				send(sock, (char *)directionbuf, 6, 0);
				m_MSG.AddString(L"Button down!\n");
				/*memcpy(m_clientSocket.m_szBuffer, directionbuf, 6);
				m_clientSocket.m_nLength = 6;
				m_clientSocket.AsyncSelect(FD_WRITE);*/
			}
		}
		if (pMsg->hwnd == GetDlgItem(IDC_BTN_RIGHT)->m_hWnd)
		{
			// 在此调用Button按钮抬起的操作
			if (m_clientSocket.m_bConnected)
			{
				uint8_t check;
				directionbuf[4] = 0x00;
				check = directionbuf[3] + directionbuf[4];
				check = ~check;
				directionbuf[5] = check;
				send(sock, (char *)directionbuf, 6, 0);
				m_MSG.AddString(L"Button down!\n");
				/*memcpy(m_clientSocket.m_szBuffer, directionbuf, 6);
				m_clientSocket.m_nLength = 6;
				m_clientSocket.AsyncSelect(FD_WRITE);*/
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CleapmotionDlg::DataToHex(int data, uint8_t flag)
{
	uint8_t buffer;
	buffer = (data % 100);              //低位
	DataHex[flag] = (uint8_t)buffer;
	buffer = (data / 100 % 100);        //高位
	DataHex[flag + 1] = (uint8_t)buffer;
}
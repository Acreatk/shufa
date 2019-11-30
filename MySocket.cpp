#include "stdafx.h"
#include "MySocket.h"
#include "leapmotion.h"
#include "leapmotionDlg.h"

MySocket::MySocket()
{
	m_bConnected = false;
	m_nLength = 0;
	m_szbuffer = L"";
	memset(m_szBuffer, 0, sizeof(m_szBuffer));
}

MySocket::~MySocket()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		Close();
	}
}


void MySocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CAsyncSocket::OnClose(nErrorCode);
}


void MySocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (nErrorCode == 0)
	{
		m_bConnected = TRUE;
		CleapmotionApp* pApp = (CleapmotionApp*)AfxGetApp();
		CleapmotionDlg* pDlg = (CleapmotionDlg*)pApp->m_pMainWnd;
		//memcpy(m_szBuffer, "Connected to ", 13);
		//strcpy_s(m_szBuffer, pDlg->m_szServerAdr);
		m_szbuffer.Format(L"Connected to %s", pDlg->m_szServerAdr);		
		pDlg->m_MSG.AddString(m_szbuffer);
		AsyncSelect(FD_READ);
	}
	CAsyncSocket::OnConnect(nErrorCode);
}


void MySocket::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	Send(m_szBuffer, m_nLength, 0);
	//Send("zaizai", 7, 0);
	m_nLength = 0;
	//memset(m_szBuffer, 0, sizeof(m_szBuffer));
	//继续提请一个“读”的网络事件，接收Server消息
	AsyncSelect(FD_READ);
	CAsyncSocket::OnSend(nErrorCode);
}

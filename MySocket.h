#pragma once

// MySocket ÃüÁîÄ¿±ê

class MySocket : public CAsyncSocket
{
public:
	MySocket();
	virtual ~MySocket();
	BOOL m_bConnected;
	UINT m_nLength;
	char m_szBuffer[4096];
	CString m_szbuffer;
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};



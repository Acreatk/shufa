#pragma once


// DlgServer �Ի���

class DlgServer : public CDialog
{
	DECLARE_DYNAMIC(DlgServer)

public:
	DlgServer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgServer();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGSERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

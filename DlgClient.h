#pragma once


// DlgClient �Ի���

class DlgClient : public CDialog
{
	DECLARE_DYNAMIC(DlgClient)

public:
	DlgClient(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgClient();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGCLIENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
};

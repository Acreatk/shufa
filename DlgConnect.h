#pragma once


// CDlgConnect �Ի���

class CDlgConnect : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConnect)

public:
	CDlgConnect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgConnect();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGCONNECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Addr;
	//int m_Port;
	CString m_Port;
	/*CHAR m_Addr;
	CHAR m_Port;*/
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeAddr();
	afx_msg void OnBnClickedOk();
};

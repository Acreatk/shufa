#pragma once


// DlgLocal �Ի���

class DlgLocal : public CDialog
{
	DECLARE_DYNAMIC(DlgLocal)

public:
	DlgLocal(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgLocal();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGLOCAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_portname;
	CString m_baudrate;
	CString m_databit;
	CString m_stopbit;
	CString m_checkbit;
//	afx_msg void OnBnClickedOklocal();
//	afx_msg void OnBnClickedOklocal();
	virtual void OnOK();
};

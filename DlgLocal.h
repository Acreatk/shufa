#pragma once


// DlgLocal 对话框

class DlgLocal : public CDialog
{
	DECLARE_DYNAMIC(DlgLocal)

public:
	DlgLocal(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgLocal();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGLOCAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

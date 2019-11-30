#pragma once


// DlgClient 对话框

class DlgClient : public CDialog
{
	DECLARE_DYNAMIC(DlgClient)

public:
	DlgClient(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgClient();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGCLIENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
};

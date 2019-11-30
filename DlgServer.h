#pragma once


// DlgServer 对话框

class DlgServer : public CDialog
{
	DECLARE_DYNAMIC(DlgServer)

public:
	DlgServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgServer();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGSERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

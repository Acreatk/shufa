// DlgConnect.cpp : 实现文件
//

#include "stdafx.h"
#include "leapmotion.h"
#include "DlgConnect.h"
#include "afxdialogex.h"
#include "Resource.h"


// CDlgConnect 对话框

IMPLEMENT_DYNAMIC(CDlgConnect, CDialogEx)

CDlgConnect::CDlgConnect(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLGCONNECT, pParent)
	, m_Addr(_T(""))
	//, m_Port(0)
	, m_Port(_T(""))
{

}

CDlgConnect::~CDlgConnect()
{
}

void CDlgConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Addr, m_Addr);
	DDX_Text(pDX, IDC_Port, m_Port);
}


BEGIN_MESSAGE_MAP(CDlgConnect, CDialogEx)
	ON_EN_CHANGE(IDC_Addr, &CDlgConnect::OnEnChangeAddr)
	ON_BN_CLICKED(IDOK, &CDlgConnect::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgConnect 消息处理程序


BOOL CDlgConnect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CEdit*)GetDlgItem(IDC_Addr))->SetWindowTextW(L"fe80::6b97:6a3:1daf:370f");
	((CEdit*)GetDlgItem(IDC_Port))->SetWindowTextW(L"10008");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgConnect::OnEnChangeAddr()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CDlgConnect::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

// DlgClient.cpp : 实现文件
//

#include "stdafx.h"
#include "leapmotion.h"
#include "DlgClient.h"
#include "afxdialogex.h"
#include "resource.h"

// DlgClient 对话框

IMPLEMENT_DYNAMIC(DlgClient, CDialog)

DlgClient::DlgClient(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLGCLIENT, pParent)
{

}

DlgClient::~DlgClient()
{
}

void DlgClient::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgClient, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &DlgClient::OnEnChangeEdit1)
END_MESSAGE_MAP()


// DlgClient 消息处理程序


void DlgClient::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

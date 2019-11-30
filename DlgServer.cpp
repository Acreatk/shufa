// DlgServer.cpp : 实现文件
//

#include "stdafx.h"
#include "leapmotion.h"
#include "DlgServer.h"
#include "afxdialogex.h"
#include "resource.h"

// DlgServer 对话框

IMPLEMENT_DYNAMIC(DlgServer, CDialog)

DlgServer::DlgServer(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLGSERVER, pParent)
{

}

DlgServer::~DlgServer()
{
}

void DlgServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgServer, CDialog)
END_MESSAGE_MAP()


// DlgServer 消息处理程序

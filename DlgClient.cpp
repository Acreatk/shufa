// DlgClient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "leapmotion.h"
#include "DlgClient.h"
#include "afxdialogex.h"
#include "resource.h"

// DlgClient �Ի���

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


// DlgClient ��Ϣ�������


void DlgClient::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

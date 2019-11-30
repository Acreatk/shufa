// DlgConnect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "leapmotion.h"
#include "DlgConnect.h"
#include "afxdialogex.h"
#include "Resource.h"


// CDlgConnect �Ի���

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


// CDlgConnect ��Ϣ�������


BOOL CDlgConnect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CEdit*)GetDlgItem(IDC_Addr))->SetWindowTextW(L"fe80::6b97:6a3:1daf:370f");
	((CEdit*)GetDlgItem(IDC_Port))->SetWindowTextW(L"10008");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgConnect::OnEnChangeAddr()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDlgConnect::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}

// DlgLocal.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "leapmotion.h"
#include "DlgLocal.h"
#include "afxdialogex.h"
#include "resource.h"

// DlgLocal �Ի���

IMPLEMENT_DYNAMIC(DlgLocal, CDialog)

DlgLocal::DlgLocal(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLGLOCAL, pParent)
	, m_portname(_T(""))
	, m_baudrate(_T(""))
	, m_databit(_T(""))
	, m_stopbit(_T(""))
	, m_checkbit(_T(""))
{

}

DlgLocal::~DlgLocal()
{
}

void DlgLocal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO1, m_portname);
	DDX_CBString(pDX, IDC_COMBO2, m_baudrate);
	DDX_CBString(pDX, IDC_COMBO3, m_databit);
	DDX_CBString(pDX, IDC_COMBO4, m_stopbit);
	DDX_CBString(pDX, IDC_COMBO5, m_checkbit);
}


BEGIN_MESSAGE_MAP(DlgLocal, CDialog)
END_MESSAGE_MAP()


// DlgLocal ��Ϣ�������


BOOL DlgLocal::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString str;

	//���ںŵ�ѡ�����
	for (int i = 0; i < 10; i++)
	{
		str.Format(L"COM%d", i);
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(str);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);//Ĭ��ѡ��COM0

	//������ѡ������
	for (int n = 0; n < 6; n++)
	{
		str.Format(L"%d", (int)pow(2, n) * 1200);
		((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(str);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(L"57600");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(L"115200");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(L"128000");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(L"256000");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(3);//Ĭ��ѡ��9600

	//����λѡ������
	for (int t = 5; t < 9; t++)
	{
		str.Format(L"%d", t);
		((CComboBox*)GetDlgItem(IDC_COMBO3))->AddString(str);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO3))->SetCurSel(3);//Ĭ��ѡ��8

	//ֹͣλѡ������
	((CComboBox*)GetDlgItem(IDC_COMBO4))->AddString(L"None");
	((CComboBox*)GetDlgItem(IDC_COMBO4))->AddString(L"1");
	((CComboBox*)GetDlgItem(IDC_COMBO4))->AddString(L"1.5");
	((CComboBox*)GetDlgItem(IDC_COMBO4))->AddString(L"2");
	((CComboBox*)GetDlgItem(IDC_COMBO4))->SetCurSel(1);//Ĭ��ѡ��1

	//����λѡ������
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString(L"None");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString(L"Odd");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString(L"Even");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString(L"Space");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString(L"Mark");
	((CComboBox*)GetDlgItem(IDC_COMBO5))->SetCurSel(0);//Ĭ��ѡ��1
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void DlgLocal::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	int index;
	CString str;
	index = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO1))->GetLBText(index, m_portname);

	index = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO2))->GetLBText(index, m_baudrate);

	index = ((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO3))->GetLBText(index, m_databit);

	index = ((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO4))->GetLBText(index, m_stopbit);

	index = ((CComboBox*)GetDlgItem(IDC_COMBO5))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO5))->GetLBText(index, m_checkbit);

	CDialog::OnOK();
}

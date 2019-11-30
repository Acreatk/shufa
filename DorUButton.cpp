#include "stdafx.h"
#include "DorUButton.h"

//#ifdef _DEBUG  
//#define new DEBUG_NEW  
//#undef THIS_FILE  
//static char THIS_FILE[] = __FILE__;
//#endif

CDorUButton::CDorUButton()
{
}


CDorUButton::~CDorUButton()
{
}


BEGIN_MESSAGE_MAP(CDorUButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



/************************************************* 
函数名:        OnLButtonDown 
函数描述:   鼠标左键按下响应函数 
输入参数:    
输出参数:                
返回值:
*************************************************/
void CDorUButton::OnLButtonDown(UINT nFlags, CPoint point)
{

	CButton::OnLButtonDown(nFlags, point);
}
/************************************************* 
函数名:        OnLButtonUp 
函数描述:   鼠标左键弹起响应函数 
输入参数:    
输出参数:                
返回值:         
*************************************************/
void CDorUButton::OnLButtonUp(UINT nFlags, CPoint point)
{

	CButton::OnLButtonUp(nFlags, point);
}






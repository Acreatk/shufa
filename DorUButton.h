#ifndef  _DUBUTTON_H
#define _DUBUTTON_H


#pragma once
#include "afxwin.h"

class CDorUButton :
	public CButton
{
public:
	CDorUButton();
	virtual ~CDorUButton();
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//按下和弹起事件  
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#endif // ! _DUBUTTON_H
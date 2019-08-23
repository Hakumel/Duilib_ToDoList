#pragma once
#include "DuilibAfx.h"
class CShadowWnd : public CWindowWnd
{
public:
	CShadowWnd()=default;
	LPCTSTR GetWindowClassName() const{ return _T("UIShadow"); }
	//UINT GetClassStyle() const;
	//void OnFinalMessage(HWND /*hWnd*/);
	//void RePaint();
	//LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	bool m_bNeedUpdate;
	CPaintManagerUI m_pm;
};
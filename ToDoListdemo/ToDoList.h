//可删
#pragma once
#include "DuilibAfx.h"

//前置声明
class CShadowWnd;

//ToDoList主界面类
class TDLuiFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	TDLuiFrameWnd();
	void Init(CControlUI* pOwner, POINT pt);
	void AdjustPostion();
	void OnFinalMessage(HWND /*hWnd*/);
	
	virtual LPCTSTR GetWindowClassName() const override;
	virtual void    Notify(TNotifyUI& msg) override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	void ShowWindow(bool bShow = true, bool bTakeFocus = true);
	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
	CPaintManagerUI m_PaintManager;
	CControlUI* m_pOwner;
	CShadowWnd* m_pShadowWnd;
	POINT m_ptPos;
};


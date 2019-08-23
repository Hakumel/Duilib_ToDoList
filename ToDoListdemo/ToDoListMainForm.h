#pragma once
#include "DuilibAfx.h"
#include <vector>
#include <string>

struct Prama
{
	HWND hWnd;
	CListUI* pList;
	CButtonUI* pCreate;
	CDuiString tstext;
};

class ToDoListMainForm : public CWindowWnd, public INotifyUI, public IListCallbackUI
{
public:
	ToDoListMainForm();
	LPCTSTR GetWindowClassName() const override;
	UINT GetClassStyle() const override;
	void OnFinalMessage(HWND  )override;
	void Init();
	void OnPrepare(TNotifyUI& msg);
	static DWORD WINAPI Create_item(LPVOID lpParameter);
	void OnCreateitem();
	LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);
	void Notify(TNotifyUI& msg);
	
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CPaintManagerUI m_PaintManager;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pCreateBtn;
	CEditUI* m_pEdit;
	static std::vector<std::string> m_vecdata;//存放事务文本
};


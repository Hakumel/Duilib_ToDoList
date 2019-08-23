#include "TodoListMainForm.h"
#include <memory>
#include <algorithm>

const int WM_ADDLISTITEM = WM_USER + 66;

//静态成员初化
std::map<unsigned, std::string> ToDoListMainForm::m_mapdata;
unsigned ToDoListMainForm::index=0;

//成员函数定义
ToDoListMainForm::ToDoListMainForm() = default;
inline LPCTSTR ToDoListMainForm::GetWindowClassName() const
{
	return _T("ToDoListMainForm");
}
inline UINT ToDoListMainForm::GetClassStyle() const
{
	return CS_DBLCLKS;
}
inline void ToDoListMainForm::OnFinalMessage(HWND)
{
	delete this;
}
void ToDoListMainForm::Init()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));
	m_pCreateBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("createbtn")));
	m_pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("InputEditUI")));
}
void ToDoListMainForm::OnPrepare(TNotifyUI& msg)
{
}
DWORD __stdcall ToDoListMainForm::Create_item(LPVOID lpParameter)
{
	try
	{
		Prama* prama = static_cast<Prama*>(lpParameter);
		CListUI* pList = prama->pList;
		CButtonUI* pCreate = prama->pCreate;
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->ApplyAttributeList(_T("height=\"45\""));
		//pListElement->SetText(0, prama->tstext);
		for (; index < static_cast<unsigned>(-1);)
		{
			if (m_mapdata.count(index) == 0)
			{
				m_mapdata[index] = prama->tstext;
				pListElement->SetTag(index++);
				break;
			}
			else
			{
				++index;
			}
		}
		if (pListElement != NULL)
		{
			::PostMessage(prama->hWnd, WM_ADDLISTITEM, 0L, (LPARAM)pListElement);
		}
		delete prama;
		return 0;
	}
	catch (exception )
	{
		return 0;
	}
}
void ToDoListMainForm::OnCreateitem()
{
	Prama* prama=new Prama();
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("todolist")));
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("InputEditUI")));
	CDuiString input = pEdit->GetText();
	DWORD dwThreadID = 0;
	pList->SetTextCallback(this);
	
	prama->hWnd = GetHWND();
	prama->pList = pList;
	prama->pCreate = m_pCreateBtn;
	prama->tstext = input;

	HANDLE hThread = CreateThread(NULL, 0, &ToDoListMainForm::Create_item, static_cast<LPVOID>(prama), 0, &dwThreadID);
}
//list的行号和列号
LPCTSTR ToDoListMainForm::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	char arr[MAX_PATH]{ '\0' };
	switch (iSubItem)
	{
	case 0:
		_stprintf_s(arr, "%d", iIndex);
		break;
	case 1:
		if (m_mapdata[iIndex].size() >= MAX_PATH-2)
		{
			strcat_s(arr, m_mapdata[iIndex].substr(0, MAX_PATH-2).c_str());
		}
		else
		{
			strcat_s(arr, m_mapdata[iIndex].c_str());
		}
		break;
	}
	pControl->SetUserData(arr);
	return pControl->GetUserData();
}
void ToDoListMainForm::Notify(TNotifyUI & msg)
{
	if (msg.sType == _T("windowinit"))
		OnPrepare(msg);
	else if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_pCloseBtn)
		{
			PostQuitMessage(0);
			return;
		}
		else if (msg.pSender == m_pMinBtn)
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return;
		}
		else if (msg.pSender == m_pCreateBtn)
		{
			OnCreateitem();
		}
	}
	else if (msg.sType == _T("itemactivate"))
	{
		//双击响应删除(bug)
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("todolist")));
		string Message = "已完成任务:";
		if (pList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcscmp(msg.pSender->GetClass(), _T("ListTextElementUI")) == 0) 
			{
				int i = pList->GetCurSel();
				CControlUI*  element=pList->GetItemAt(i);
				unsigned tag = element->GetTag();
				Message+=m_mapdata[tag];
				m_mapdata.erase(tag);
				pList->Remove(element);
				pList->SetTextCallback(this);
			}
		}
/*
		string Message = "任务信息:";
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("todolist")));
		if (pList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcscmp(msg.pSender->GetClass(), _T("ListTextElementUI")) == 0)
			{
				int i = pList->GetCurSel();
				CControlUI* element = pList->GetItemAt(i);
				unsigned tag = element->GetTag();
				Message+=m_mapdata[tag];
			}
		}*/
		
		::MessageBox(NULL, Message.c_str(), _T("提示"), MB_OK);
	}
}

LRESULT ToDoListMainForm::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_ADDLISTITEM:   lRes = OnAddListItem(uMsg, wParam, lParam, bHandled); break;
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ToDoListMainForm::OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	CListTextElementUI* pListElement =(CListTextElementUI*)lParam;
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("todolist")));
	if (pList)  pList->Add(pListElement);

	return 0;
}

LRESULT ToDoListMainForm::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_PaintManager.Init(m_hWnd);
	m_PaintManager.SetTransparent(260);
	CDialogBuilder builder;
	CControlUI * pRoot = builder.Create(_T("skin.xml"), (UINT)0, NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	Init();
	return 0;
}

inline LRESULT ToDoListMainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT ToDoListMainForm::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

LRESULT ToDoListMainForm::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

inline LRESULT ToDoListMainForm::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return 0;
}

inline LRESULT ToDoListMainForm::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	return 0;
}

LRESULT ToDoListMainForm::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	if (!::IsZoomed(*this)) {
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top) {
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
	}
	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("TextUI")) != 0)
			return HTCAPTION;
	}
	return HTCLIENT;
}

LRESULT ToDoListMainForm::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) 
	{
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		RECT rc = { rcWnd.left, rcWnd.top + szRoundCorner.cx, rcWnd.right, rcWnd.bottom };
		HRGN hRgn1 = ::CreateRectRgnIndirect(&rc);
		HRGN hRgn2 = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
		::CombineRgn(hRgn1, hRgn1, hRgn2, RGN_OR);
		::SetWindowRgn(*this, hRgn1, TRUE);
		::DeleteObject(hRgn1);
		::DeleteObject(hRgn2);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT ToDoListMainForm::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT ToDoListMainForm::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

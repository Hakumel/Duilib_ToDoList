#include "ToDoList.h"
#include "CShadowWnd.h"

TDLuiFrameWnd::TDLuiFrameWnd() :m_pOwner(nullptr), m_pShadowWnd(nullptr), m_ptPos()
{
}

void TDLuiFrameWnd::Init(CControlUI* pOwner, POINT pt)
{
	if (pOwner == NULL) return;
	m_pOwner = pOwner;
	m_ptPos = pt;
	Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
	HWND hWndParent = m_hWnd;
	while (::GetParent(hWndParent) != NULL) hWndParent = ::GetParent(hWndParent);
	::ShowWindow(m_hWnd, SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

void TDLuiFrameWnd::AdjustPostion()
{
	CDuiRect rcWnd;
	GetWindowRect(m_hWnd, &rcWnd);
	int nWidth = rcWnd.GetWidth();
	int nHeight = rcWnd.GetHeight();
	rcWnd.left = m_ptPos.x;
	rcWnd.top = m_ptPos.y;
	rcWnd.right = rcWnd.left + nWidth;
	rcWnd.bottom = rcWnd.top + nHeight;
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;

	if (rcWnd.bottom > rcWork.bottom) {
		if (nHeight >= rcWork.GetHeight()) {
			rcWnd.top = 0;
			rcWnd.bottom = nHeight;
		}
		else {
			rcWnd.bottom = rcWork.bottom;
			rcWnd.top = rcWnd.bottom - nHeight;
		}
	}
	if (rcWnd.right > rcWork.right) {
		if (nWidth >= rcWork.GetWidth()) {
			rcWnd.left = 0;
			rcWnd.right = nWidth;
		}
		else {
			rcWnd.right = rcWork.right;
			rcWnd.left = rcWnd.right - nWidth;
		}
	}
	::SetWindowPos(m_hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

inline void TDLuiFrameWnd::OnFinalMessage(HWND)
{
	 delete this; 
}

inline LPCTSTR TDLuiFrameWnd::GetWindowClassName() const
{
	return _T("TDLuiFrameWnd");
}

void TDLuiFrameWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("itemselect")) {
		Close();
	}
	else if (msg.sType == _T("itemclick")) {
		if (msg.pSender->GetName() == _T("menu_Delete")) {
			if (m_pOwner) m_pOwner->GetManager()->SendNotify(m_pOwner, _T("menu_Delete"), 0, 0, true);
		}
	}
	
}

LRESULT TDLuiFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	if (uMsg == WM_CREATE)
	{
		CControlUI* pWnd = new CButtonUI;
		pWnd->SetName(_T("btmAddList"));      // 设置控件的名称，这个名称用于标识每一个控件，必须唯一，相当于MFC里面的控件
		pWnd->SetText(_T("事务清单"));   // 设置文字
		pWnd->SetBkColor(0xFF00FF00);       // 设置背景色

		m_PaintManager.Init(m_hWnd);
		m_PaintManager.AttachDialog(pWnd);
		m_PaintManager.AddNotifier(this);   // 添加控件等消息响应，这样消息就会传达到duilib的消息循环，我们可以在Notify函数里做消息处理
		return lRes;
	}
	//屏蔽自带标题栏
	else if (uMsg == WM_NCACTIVATE)
	{
		if (!::IsIconic(m_hWnd))
		{
			return (wParam == 0) ? TRUE : FALSE;
		}
	}
	else if (uMsg == WM_NCCALCSIZE)
	{
		return 0;
	}
	else if (uMsg == WM_NCPAINT)
	{
		return 0;
	}
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEWHEEL:    break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_MOVE:          lRes = OnMove(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void TDLuiFrameWnd::ShowWindow(bool bShow, bool bTakeFocus)
{
	if (m_pShadowWnd != NULL) m_pShadowWnd->ShowWindow(bShow, false);
	CWindowWnd::ShowWindow(bShow, bTakeFocus);
}

HWND TDLuiFrameWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
{
	if (m_pShadowWnd == NULL) m_pShadowWnd = new CShadowWnd;
	m_pShadowWnd->Create(hwndParent, _T(""), WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS, WS_EX_LAYERED | WS_EX_TOOLWINDOW, x, y, cx, cy, NULL);

	dwExStyle |= WS_EX_TOOLWINDOW;
	return CWindowWnd::Create(hwndParent, pstrName, dwStyle, dwExStyle, x, y, cx, cy, hMenu);
}

LRESULT TDLuiFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("menu.xml"), (UINT)0, NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);
	m_PaintManager.SetRoundCorner(3, 3);

	CListUI* pList = static_cast<CListUI*>(m_pOwner);
	int nSel = pList->GetCurSel();
	if (nSel < 0) {
		CControlUI* pControl = m_PaintManager.FindControl(_T("menu_Delete"));
		if (pControl) pControl->SetEnabled(false);
	}

	AdjustPostion();
	return 0;
}

LRESULT TDLuiFrameWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pShadowWnd != NULL) m_pShadowWnd->Close();
	bHandled = FALSE;
	return 0;
}

LRESULT TDLuiFrameWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((HWND)wParam == m_hWnd) bHandled = TRUE;
	else if (m_pShadowWnd != NULL && (HWND)wParam == m_pShadowWnd->GetHWND()) {
		CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		::SetFocus(m_hWnd);
		bHandled = TRUE;
	}
	else {
		Close();
		bHandled = FALSE;
	}
	return 0;
}

inline LRESULT TDLuiFrameWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == VK_ESCAPE) Close();
	return 0;
}

LRESULT TDLuiFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!::IsIconic(*this)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		HRGN hRgn = ::CreateRectRgn(rcWnd.left + 8, rcWnd.top + 8, rcWnd.right - 8, rcWnd.bottom - 8);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
	if (m_pShadowWnd != NULL) {
		RECT rcWnd = { 0 };
		::GetWindowRect(m_hWnd, &rcWnd);
		::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left,
			rcWnd.bottom - rcWnd.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT TDLuiFrameWnd::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pShadowWnd != NULL) {
		RECT rcWnd = { 0 };
		::GetWindowRect(m_hWnd, &rcWnd);
		::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left,
			rcWnd.bottom - rcWnd.top, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
	}
	bHandled = FALSE;
	return 0;
}

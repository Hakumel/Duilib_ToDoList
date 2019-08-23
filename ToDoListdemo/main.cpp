#pragma once
#include "TodoListMainForm.h"
#include "StdAfx.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Skin\\ToDoListRes"));  
	ToDoListMainForm Frame;
	Frame.Create(NULL, _T("ToDoListDemo"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	Frame.CenterWindow();
	::ShowWindow(Frame, SW_SHOW);
	CPaintManagerUI::MessageLoop();
	return 0;
}
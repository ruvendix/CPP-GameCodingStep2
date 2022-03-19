#include "PCH.h"
#include "WindowProcedure.h"

#include "Config/Config.h"
#include "WindowApp/WindowApp.h"

#include "ErrorHandler/ErrorHandler.h"

WindowProcedure::WindowProcedure()
{
	
}

WindowProcedure::~WindowProcedure()
{
	
}

LRESULT CALLBACK WindowProcedure::CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// �޽��� ũ��Ŀ �б��Դϴ�.
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
		HANDLE_MSG(hWnd, WM_SETFOCUS, OnSetFocus);
		HANDLE_MSG(hWnd, WM_KILLFOCUS, OnKillFocus);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWN, OnSysKeyDown);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
	}

	// ��κ��� �޽����� �ü������ �����ϴ�.
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL WindowProcedure::OnCreate(HWND hWnd, CREATESTRUCT* pCreateStruct)
{
	return TRUE; // �޽��� ũ��Ŀ�� ���� �ݵ�� TRUE�� ��ȯ�ؾ� �մϴ�.
}

void WindowProcedure::OnDestroy(HWND hWnd)
{
	// WM_DESTROY�� ������ WM_QUIT�� �����ϴ�.
	::PostQuitMessage(EXIT_SUCCESS);
}

void WindowProcedure::OnSetFocus(HWND hWnd, HWND hOldFoucsWnd)
{
	if (m_bAltTabMinimize == true)
	{
		m_bAltTabMinimize = !m_bAltTabMinimize;
		m_pWndApp->ToggleAltTabState(m_bAltTabMinimize);
	}
}

void WindowProcedure::OnKillFocus(HWND hWnd, HWND hNewFocusWnd)
{
	if (m_bAltTabMinimize == false)
	{
		m_bAltTabMinimize = !m_bAltTabMinimize;
		m_pWndApp->ToggleAltTabState(m_bAltTabMinimize);
	}
}

void WindowProcedure::OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags)
{
	
}

void WindowProcedure::OnSysKeyDown(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags)
{
	if ((flags & KF_ALTDOWN) &&
		(virtualKeyCode == VK_RETURN))
	{
		m_pWndApp->ToggleScreenMode();
	}
}

void WindowProcedure::OnSize(HWND hWnd, UINT state, int width, int height)
{
	::SetWindowPos(hWnd, nullptr, 0, 0, m_pConfig->GetClientWidth(), m_pConfig->GetClientHeight(), SWP_SHOWWINDOW);
	::ShowWindow(hWnd, SW_SHOW);
}
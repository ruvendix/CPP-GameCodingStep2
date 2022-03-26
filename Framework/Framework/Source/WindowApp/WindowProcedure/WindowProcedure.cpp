#include "PCH.h"
#include "WindowProcedure.h"

#include "Config/Config.h"
#include "WindowApp/WindowApp.h"

#include "ErrorHandler/ErrorHandler.h"
#include "InputDevice/InputManager.h"

WindowProcedure::WindowProcedure()
{
	
}

WindowProcedure::~WindowProcedure()
{
	
}

LRESULT CALLBACK WindowProcedure::CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 메시지 크래커 분기입니다.
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_WINDOWPOSCHANGING, OnWindowPosChanging);

		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_KEYDOWN, OnKeyDown);
		HANDLE_MSG(hWnd, WM_KEYUP, OnKeyUp);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
		HANDLE_MSG(hWnd, WM_SETFOCUS, OnSetFocus);
		HANDLE_MSG(hWnd, WM_KILLFOCUS, OnKillFocus);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWN, OnSysKeyDown);
		HANDLE_MSG(hWnd, WM_SYSKEYUP, OnSysKeyUp);
	}

	// 대부분의 메시지는 운영체제에게 보냅니다.
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL WindowProcedure::OnCreate(HWND hWnd, CREATESTRUCT* pCreateStruct)
{
	return TRUE; // 메시지 크래커로 인해 반드시 TRUE를 반환해야 합니다.
}

void WindowProcedure::OnDestroy(HWND hWnd)
{
	// WM_DESTROY를 받으면 WM_QUIT을 보냅니다.
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

void WindowProcedure::OnKeyDown(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags)
{
	SINGLETON(InputManager)->PushInputEvent(InputEvent{ static_cast<UINT8>(virtualKeyCode), EInputState::DOWN });
}

void WindowProcedure::OnKeyUp(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags)
{
	SINGLETON(InputManager)->PushInputEvent(InputEvent{ static_cast<UINT8>(virtualKeyCode), EInputState::UP });
}

void WindowProcedure::OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags)
{
	
}

void WindowProcedure::OnSysKeyDown(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags)
{
	if (flags & KF_ALTDOWN)
	{
		if (virtualKeyCode == VK_RETURN)
		{
			m_pWndApp->ToggleScreenMode();
		}
		else if (virtualKeyCode == VK_F4)
		{
			::PostQuitMessage(EXIT_SUCCESS);
		}
	}
}

void WindowProcedure::OnSysKeyUp(HWND hwnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags)
{

}

BOOL WindowProcedure::OnWindowPosChanging(HWND hWnd, LPWINDOWPOS lpwpos)
{
	lpwpos->cx = m_pConfig->GetWindowWidth();
	lpwpos->cy = m_pConfig->GetWindowHeight();

	return TRUE;
}
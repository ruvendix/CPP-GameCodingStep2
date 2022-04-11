#include "PCH.h"
#include "WindowProcedure.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Config/Config.h"

LRESULT CALLBACK WindowProcedure::CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 메시지 크래커 분기입니다.
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_WINDOWPOSCHANGING, OnWindowPosChanging);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWN, OnSysKeyDown);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
	}

	// 대부분의 메시지는 운영체제에게 보냅니다.
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL WindowProcedure::OnWindowPosChanging(HWND hWnd, LPWINDOWPOS lpwpos)
{
	lpwpos->cx = SINGLETON(Config)->GetWindowWidth();
	lpwpos->cy = SINGLETON(Config)->GetWindowHeight();

	std::string str = "X: " + std::to_string(lpwpos->x) + " " + std::to_string(lpwpos->y) + "\n";
	::OutputDebugString(str.c_str());

	return TRUE;
}

BOOL WindowProcedure::OnCreate(HWND hWnd, CREATESTRUCT* pCreateStruct)
{
	return TRUE; // 메시지 크래커로 인해 반드시 TRUE를 반환해야 합니다.
}

void WindowProcedure::OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags)
{
	CUSTOM_ERROR("왼쪽 마우스 버튼이 클릭되었을 때 오류 확인!");
}

void WindowProcedure::OnSetFocus(HWND hWnd, HWND hOldFoucsWnd)
{
	SINGLETON(Config)->ToggleAltTabState(hWnd);
}

void WindowProcedure::OnKillFocus(HWND hWnd, HWND hNewFocusWnd)
{
	SINGLETON(Config)->ToggleAltTabState(hWnd);
}

void WindowProcedure::OnSysKeyDown(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags)
{
	if (flags & KF_ALTDOWN)
	{
		if (virtualKeyCode == VK_RETURN)
		{
			SINGLETON(Config)->AutoChangeScreenMode(hWnd);
		}
		else if (virtualKeyCode == VK_F4)
		{
			::PostQuitMessage(EXIT_SUCCESS);
		}
	}
}

void WindowProcedure::OnDestroy(HWND hWnd)
{
	// WM_DESTROY를 받으면 WM_QUIT을 보냅니다.
	::PostQuitMessage(EXIT_SUCCESS);
}
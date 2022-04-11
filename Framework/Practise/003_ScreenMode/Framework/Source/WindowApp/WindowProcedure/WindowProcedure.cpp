#include "PCH.h"
#include "WindowProcedure.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Config/Config.h"

LRESULT CALLBACK WindowProcedure::CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// �޽��� ũ��Ŀ �б��Դϴ�.
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_WINDOWPOSCHANGING, OnWindowPosChanging);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
		HANDLE_MSG(hWnd, WM_SYSKEYDOWN, OnSysKeyDown);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
	}

	// ��κ��� �޽����� �ü������ �����ϴ�.
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
	return TRUE; // �޽��� ũ��Ŀ�� ���� �ݵ�� TRUE�� ��ȯ�ؾ� �մϴ�.
}

void WindowProcedure::OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags)
{
	CUSTOM_ERROR("���� ���콺 ��ư�� Ŭ���Ǿ��� �� ���� Ȯ��!");
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
	// WM_DESTROY�� ������ WM_QUIT�� �����ϴ�.
	::PostQuitMessage(EXIT_SUCCESS);
}
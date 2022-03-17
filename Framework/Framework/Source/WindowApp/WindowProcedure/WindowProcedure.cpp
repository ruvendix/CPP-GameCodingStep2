#include "PCH.h"
#include "WindowProcedure.h"

#include "Config/Config.h"
#include "WindowApp/WindowApp.h"

#include "ErrorHandler/ErrorHandler.h"

LRESULT WindowProcedure::CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 메시지 크래커 스위치입니다.
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);

	case WM_SYSKEYDOWN:
		{
			if (((HIWORD(lParam) & KF_ALTDOWN)) &&
				(wParam == VK_RETURN))				
			{
				m_pWndApp->ToggleScreenMode();
			}
			break;
		}

	case WM_SIZE:
	{
		::SetWindowPos(hWnd, nullptr, 0, 0, m_pConfig->GetClientWidth(), m_pConfig->GetClientHeight(), SWP_SHOWWINDOW);
		break;
	}
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

void WindowProcedure::OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags)
{
	::MessageBox(hWnd, "Good", "What", MB_OK);
}
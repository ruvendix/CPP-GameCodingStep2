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

	case WM_SETFOCUS:
	{
		::OutputDebugString("WM_SETFOCUS!\n");
		if (m_altTabState == EAltTabState::MINIMIZE)
		{
			::OutputDebugString("WM_SETFOCUS! Alt Tab ����!\n");
			TOGGLE_ENUM(m_altTabState, EAltTabState);
			m_pWndApp->ToggleAltTabState(m_altTabState);
		}

		break;
	}

	case WM_KILLFOCUS:
	{
		::OutputDebugString("WM_KILLFOCUS!\n");
		if (m_altTabState == EAltTabState::NORMAL)
		{
			::OutputDebugString("WM_KILLFOCUS! Alt Tab ����!\n");
			TOGGLE_ENUM(m_altTabState, EAltTabState);
			m_pWndApp->ToggleAltTabState(m_altTabState);
		}

		break;
	}

	case WM_SYSKEYDOWN:
	{
		if ((HIWORD(lParam) & KF_ALTDOWN) &&
			(wParam == VK_RETURN))
		{
			m_pWndApp->ToggleScreenMode();
		}

		break;
	}

	case WM_SIZE:
	{
		::SetWindowPos(hWnd, nullptr, 0, 0, m_pConfig->GetClientWidth(), m_pConfig->GetClientHeight(), SWP_SHOWWINDOW);
		::ShowWindow(hWnd, SW_SHOW);

		break;
	}
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

void WindowProcedure::OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags)
{
	::MessageBox(hWnd, "Good", "What", MB_OK);
}
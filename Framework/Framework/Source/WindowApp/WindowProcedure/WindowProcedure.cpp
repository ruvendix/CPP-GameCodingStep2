#include "PCH.h"
#include "WindowProcedure.h"

LRESULT WindowProcedure::CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// �޽��� ũ��Ŀ ����ġ�Դϴ�.
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
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
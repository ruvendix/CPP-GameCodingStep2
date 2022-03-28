#include "PCH.h"
#include "WindowProcedure.h"

#include "ErrorHandler/ErrorHandler.h"

LRESULT CALLBACK WindowProcedure::CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 메시지 크래커 분기입니다.
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
	}

	// 대부분의 메시지는 운영체제에게 보냅니다.
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL WindowProcedure::OnCreate(HWND hWnd, CREATESTRUCT* pCreateStruct)
{
	return TRUE; // 메시지 크래커로 인해 반드시 TRUE를 반환해야 합니다.
}

void WindowProcedure::OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags)
{
	CUSTOM_ERROR("왼쪽 마우스 버튼이 클릭되었을 때 오류 확인!");
}

void WindowProcedure::OnDestroy(HWND hWnd)
{
	// WM_DESTROY를 받으면 WM_QUIT을 보냅니다.
	::PostQuitMessage(EXIT_SUCCESS);
}
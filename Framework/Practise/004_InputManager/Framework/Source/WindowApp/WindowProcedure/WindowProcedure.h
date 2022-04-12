#pragma once

#include "CommonInclude/Windows.h"

class WindowApp;

class WindowProcedure
{
public:
	WindowProcedure() = default;
	virtual ~WindowProcedure() = default;

	LRESULT CALLBACK CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnWindowPosChanging(HWND hWnd, LPWINDOWPOS lpwpos);
	virtual BOOL OnCreate(HWND hWnd, CREATESTRUCT* pCreateStruct);

	virtual void OnKeyDown(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags);
	virtual void OnKeyUp(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags);
	virtual void OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags);
	virtual void OnSetFocus(HWND hWnd, HWND hOldFoucsWnd);
	virtual void OnKillFocus(HWND hWnd, HWND hNewFocusWnd);
	virtual void OnSysKeyDown(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags);
	virtual void OnDestroy(HWND hWnd);
};
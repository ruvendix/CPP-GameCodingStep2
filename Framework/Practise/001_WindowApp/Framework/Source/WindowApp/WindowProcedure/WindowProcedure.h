#pragma once

#include "CommonInclude/Windows.h"

class WindowApp;

class WindowProcedure
{
public:
	WindowProcedure() = default;
	virtual ~WindowProcedure() = default;

	LRESULT CALLBACK CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnCreate(HWND hWnd, CREATESTRUCT* pCreateStruct);
	virtual void OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags);
	virtual void OnDestroy(HWND hWnd);
};
#pragma once

#include "PCH.h"

class WindowApp;
class Config;

class WindowProcedure
{
public:
	WindowProcedure() = default;
	virtual ~WindowProcedure() = default;

	LRESULT CALLBACK CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnCreate(HWND hWnd, CREATESTRUCT* pCreateStruct);
	virtual void OnDestroy(HWND hWnd);

	virtual void OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags);

	void SetConfig(Config* pConfig) { m_pConfig = pConfig; }
	void SetWindowApp(WindowApp* pWndApp) { m_pWndApp = pWndApp; }

private:
	WindowApp* m_pWndApp = nullptr;
	Config* m_pConfig = nullptr;
};
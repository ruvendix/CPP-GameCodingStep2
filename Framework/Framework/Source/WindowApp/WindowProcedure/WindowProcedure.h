#pragma once

#include "PCH.h"

class WindowApp;
class Config;

class WindowProcedure
{
public:
	WindowProcedure();
	virtual ~WindowProcedure();

	LRESULT CALLBACK CallWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnCreate(HWND hWnd, CREATESTRUCT* pCreateStruct);
	virtual BOOL OnWindowPosChanging(HWND hWnd, LPWINDOWPOS lpwpos);

	virtual void OnDestroy(HWND hWnd);
	virtual void OnSetFocus(HWND hWnd, HWND hOldFoucsWnd);
	virtual void OnKillFocus(HWND hWnd, HWND hNewFocusWnd);
	virtual void OnKeyDown(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags);
	virtual void OnKeyUp(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags);
	virtual void OnLButtonDown(HWND hWnd, BOOL bDoubleClick, INT32 x, INT32 y, UINT keyFlags);
	virtual void OnSysKeyDown(HWND hWnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags);
	virtual void OnSysKeyUp(HWND hwnd, UINT virtualKeyCode, BOOL bKeyDown, int repeat, UINT flags);

	void SetConfig(Config* pConfig) { m_pConfig = pConfig; }
	void SetWindowApp(WindowApp* pWndApp) { m_pWndApp = pWndApp; }

private:
	WindowApp* m_pWndApp = nullptr;
	Config* m_pConfig = nullptr;

	bool m_bAltTabMinimize = false;
};
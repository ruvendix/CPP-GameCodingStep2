#pragma once

#include "PCH.h"

class WindowViewer;
class WindowProcedure;
class Config;

class WindowApp
{
public:
	WindowApp(HINSTANCE hInst, const std::string& strWndClassName);
	~WindowApp();

	int Run();

	HRESULT StartUp();
	void CleanUp();

	void Do();

	static LRESULT CALLBACK TossWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//////////////////////////////////////////////////////////////////////////////
	// 내부 전용 함수
	void ApplyFullScreenWindow();

private:
	HINSTANCE m_hInst = nullptr;
	std::string m_strWndClassName;
	WNDCLASSEX m_wndClass;

	WindowViewer* m_pWndViewer = nullptr;
	WindowProcedure* m_pWndProcedure = nullptr;

	Config* m_pConfig = nullptr;

	bool m_bRun = true;
};

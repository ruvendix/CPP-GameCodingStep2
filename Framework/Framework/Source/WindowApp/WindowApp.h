#pragma once

#include "PCH.h"
#include "CommonInclude/Enum.h"

class WindowViewer;
class WindowProcedure;
class Config;

class WindowApp
{
public:
	static LRESULT CALLBACK TossWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	WindowApp(HINSTANCE hInst, const std::string& strWndClassName);
	~WindowApp();

	int Run();

	HRESULT StartUp();
	void CleanUp();

	void Do();
	void ToggleScreenMode();
	void ToggleAltTabState(EAltTabState altTabState);

private:
	HINSTANCE m_hInst = nullptr;
	std::string m_strWndClassName;
	WNDCLASSEX m_wndClass;

	WindowViewer* m_pWndViewer = nullptr;
	WindowProcedure* m_pWndProcedure = nullptr;

	Config* m_pConfig = nullptr;

	bool m_bRun = true;
};

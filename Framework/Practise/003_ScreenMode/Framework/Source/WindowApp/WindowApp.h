#pragma once

#include "CommonInclude/Windows.h"

class WindowViewer;
class WindowProcedure;

class WindowApp
{
public:
	static LRESULT CALLBACK TossWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	WindowApp(HINSTANCE hInst, const std::string& strWndClassName);
	~WindowApp();

	int Run();

	HRESULT StartUp();
	void Do();
	void CleanUp();

private:
	HINSTANCE m_hInst = nullptr;

	std::string m_strWndClassName;
	WNDCLASSEX m_wndClass;

	std::shared_ptr<WindowViewer> m_spWndViewer;
	std::shared_ptr<WindowProcedure> m_spWndProcedure;

	bool m_bRun = true;
};

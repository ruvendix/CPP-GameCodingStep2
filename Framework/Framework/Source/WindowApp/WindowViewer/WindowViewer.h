#pragma once

#include "PCH.h"

class WindowProcedure;

struct WindowCreateInfo
{
	std::string strWndClassName;
	int x = 0;
	int y = 0;
	UINT clientWidth = 0;
	UINT clientHeight = 0;
	DWORD dwStyle = 0;
	HINSTANCE hInst = nullptr;
	WindowProcedure* pWndProcedure = nullptr;
};

class WindowViewer
{
public:
	WindowViewer(const std::string& strViewerName);

	HRESULT Create(const WindowCreateInfo& wndCreateInfo);
	void Destroy();

	HWND GetWindowHandle() { return m_hWnd; }

private:
	std::string m_strViewerName;
	HWND m_hWnd = nullptr;
};
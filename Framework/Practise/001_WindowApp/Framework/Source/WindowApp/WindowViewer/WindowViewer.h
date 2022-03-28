#pragma once

#include "CommonInclude/Struct.h"
#include "CommonInclude/Windows.h"

class WindowProcedure;

struct WindowCreateInfo
{
	std::string strWndClassName;
	Position2D windowStartPos;
	UINT windowWidth = 0;
	UINT windowHeight = 0;
	DWORD dwStyle = 0;
	DWORD dwExStyle = 0;
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
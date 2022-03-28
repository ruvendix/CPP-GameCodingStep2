#include "PCH.h"
#include "WindowViewer.h"

WindowViewer::WindowViewer(const std::string& strViewerName) :
	m_strViewerName(strViewerName)
{

}

HRESULT WindowViewer::Create(const WindowCreateInfo& wndCreateInfo)
{
	// 윈도우를 생성하고 사용자에게 보여줍니다.
	m_hWnd = ::CreateWindowEx(
		WS_EX_APPWINDOW,
		wndCreateInfo.strWndClassName.c_str(),
		m_strViewerName.c_str(),
		wndCreateInfo.dwStyle,
		wndCreateInfo.windowStartPos.x,
		wndCreateInfo.windowStartPos.y,
		wndCreateInfo.windowWidth,
		wndCreateInfo.windowHeight,
		HWND_DESKTOP,
		nullptr,
		wndCreateInfo.hInst,
		wndCreateInfo.pWndProcedure);

	if (m_hWnd == nullptr)
	{
		return E_FAIL;
	}

	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	return S_OK;
}

void WindowViewer::Destroy()
{
	::DestroyWindow(m_hWnd);
}
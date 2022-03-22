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
		//ERRMSGBOX(L"윈도우 생성 실패!");
		return E_FAIL;
	}

	// 클라이언트 영역 기준으로 윈도우 영역을 재조정합니다.
	//AdjustClientArea(m_hWnd, clientWidth, clientHeight);

	// 윈도우 핸들값을 얻었으므로 클라이언트 영역 갱신이 가능합니다.
	//::GetClientRect(m_hWnd, &m_clientArea);

	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	return S_OK;
}

void WindowViewer::Destroy()
{
	::DestroyWindow(m_hWnd);
}

#include "PCH.h"
#include "WindowViewer.h"

WindowViewer::WindowViewer(const std::string& strViewerName) :
	m_strViewerName(strViewerName)
{

}

HRESULT WindowViewer::Create(const WindowCreateInfo& wndCreateInfo)
{
	// �����츦 �����ϰ� ����ڿ��� �����ݴϴ�.
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
		//ERRMSGBOX(L"������ ���� ����!");
		return E_FAIL;
	}

	// Ŭ���̾�Ʈ ���� �������� ������ ������ �������մϴ�.
	//AdjustClientArea(m_hWnd, clientWidth, clientHeight);

	// ������ �ڵ鰪�� ������Ƿ� Ŭ���̾�Ʈ ���� ������ �����մϴ�.
	//::GetClientRect(m_hWnd, &m_clientArea);

	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	return S_OK;
}

void WindowViewer::Destroy()
{
	::DestroyWindow(m_hWnd);
}

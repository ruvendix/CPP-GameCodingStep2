#include "PCH.h"
#include "WindowViewer.h"

WindowViewer::WindowViewer(const std::string& strViewerName) :
	m_strViewerName(strViewerName)
{

}

HRESULT WindowViewer::Create(const WindowViewerDescription& wndViewerDesc)
{
	// �����츦 �����ϰ� ����ڿ��� �����ݴϴ�.
	m_hWnd = ::CreateWindowEx(
		wndViewerDesc.dwExStyle,
		wndViewerDesc.strWndClassName.c_str(),
		m_strViewerName.c_str(),
		wndViewerDesc.dwStyle,
		wndViewerDesc.wndStartPos.x,
		wndViewerDesc.wndStartPos.y,
		wndViewerDesc.wndWidth,
		wndViewerDesc.wndHeight,
		HWND_DESKTOP,
		nullptr,
		wndViewerDesc.hInst,
		wndViewerDesc.pWndProcedure);

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
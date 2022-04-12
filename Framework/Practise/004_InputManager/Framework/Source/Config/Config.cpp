#include "PCH.h"
#include "Config.h"

#include "PCH.h"
#include "Config.h"

#include "ErrorHandler/ErrorHandler.h"

Config::Config()
{
	::ZeroMemory(&m_deviceMode, sizeof(DEVMODE));
	m_deviceMode.dmSize = sizeof(DEVMODE);
	m_deviceMode.dmBitsPerPel = 32; // �ȼ��� ��Ʈ��
	m_deviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // �����Ұ��� ����
}

Config::~Config()
{

}

void Config::AdjustWindowRect(DWORD dwStyle, DWORD dwExStyle)
{
	RECT resultRect;
	::ZeroMemory(&resultRect, sizeof(RECT));
	resultRect.right = m_clientWidth;
	resultRect.bottom = m_clientHeight;

	if (::AdjustWindowRectExForDpi(&resultRect, dwStyle, FALSE, dwExStyle, ::GetDpiForSystem()) == FALSE)
	{
		COM_ERROR(::GetLastError());
	}

	m_windowWidth = resultRect.right - resultRect.left;
	m_windowHeight = resultRect.bottom - resultRect.top;
}

void Config::ChangeDeviceResolution(UINT width, UINT height)
{
	m_deviceMode.dmPelsWidth = width;
	m_deviceMode.dmPelsHeight = height;
	::ChangeDisplaySettings(&m_deviceMode, CDS_UPDATEREGISTRY);
}

void Config::AutoChangeScreenMode(HWND hWndViewer)
{
	// ���� ȭ�� ���� "Alt + Enter" ��带 �ٲٸ鼭 �����մϴ�.
	EScreenMode altEnterScreenMode = m_altEnterScreenMode;
	std::swap(m_currentScreenMode, m_altEnterScreenMode);

	switch (altEnterScreenMode)
	{
	/*
	? -> â���
	���α׷� ���� ���� �����ߴ� ����� �ػ󵵷� ����, ������ �ػ󵵴� �����ߴ� ������ �����մϴ�.
	*/
	case EScreenMode::WINDOW:
	{
		ChangeDeviceResolution(m_screenWidth, m_screenHeight);

		::SetWindowLongPtr(hWndViewer, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		::SetWindowPos(hWndViewer, nullptr, m_windowStartPos.x, m_windowStartPos.y, 0, 0, SWP_NOSIZE);
		::ShowWindow(hWndViewer, SW_SHOW);

		break;
	}

	/*
	? -> ��ü â���
	�ü���� ������ ����� �ػ󵵿� ������ �ػ󵵸� ��ġ��ŵ�ϴ�.
	*/
	case EScreenMode::FULLSCREEN_WINDOW:
	{
		ChangeDeviceResolution(m_clientWidth, m_clientHeight);
		break;
	}

	/*
	? -> ��ü ȭ��
	��ü â���� ������ �˾� ������� �����ϴ� �͸� �ٸ��ϴ�.
	*/
	case EScreenMode::FULLSCREEN:
	{
		ChangeDeviceResolution(m_clientWidth, m_clientHeight);

		::SetWindowLongPtr(hWndViewer, GWL_STYLE, WS_POPUP);
		::ShowWindow(hWndViewer, SW_SHOW);

		break;
	}
	}
}

void Config::ToggleAltTabState(HWND hWndViewer)
{
	// ��ü ȭ���� ���� �ǹ̰� �ֽ��ϴ�!
	if (m_currentScreenMode != EScreenMode::FULLSCREEN)
	{
		return;
	}

	m_bAltTabMinimize = !m_bAltTabMinimize;
	if (m_bAltTabMinimize == true)
	{
		::SetWindowLongPtr(hWndViewer, GWL_STYLE, WS_MINIMIZE);
		::ShowWindow(hWndViewer, SW_SHOW);
	}
	else
	{
		// ��ü ȭ���� ���� Ŭ���̾�Ʈ ������ ������ ������ ���̰� �����ϴ�!
		ChangeDeviceResolution(m_clientWidth, m_clientHeight);

		::SetWindowLongPtr(hWndViewer, GWL_STYLE, WS_POPUP);
		::ShowWindow(hWndViewer, SW_SHOW);
	}
}

void Config::CleanUp()
{
	if (m_currentScreenMode != EScreenMode::WINDOW)
	{
		ChangeDeviceResolution(m_screenWidth, m_screenHeight);
	}
}
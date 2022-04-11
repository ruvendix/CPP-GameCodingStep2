#include "PCH.h"
#include "Config.h"

#include "PCH.h"
#include "Config.h"

#include "ErrorHandler/ErrorHandler.h"

Config::Config()
{
	::ZeroMemory(&m_deviceMode, sizeof(DEVMODE));
	m_deviceMode.dmSize = sizeof(DEVMODE);
	m_deviceMode.dmBitsPerPel = 32; // 픽셀당 비트수
	m_deviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // 설정할값들 예약
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
	// 현재 화면 모드와 "Alt + Enter" 모드를 바꾸면서 적용합니다.
	EScreenMode altEnterScreenMode = m_altEnterScreenMode;
	std::swap(m_currentScreenMode, m_altEnterScreenMode);

	switch (altEnterScreenMode)
	{
	/*
	? -> 창모드
	프로그램 실행 전에 설정했던 모니터 해상도로 복구, 윈도우 해상도는 설정했던 값으로 변경합니다.
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
	? -> 전체 창모드
	운영체제에 설정한 모니터 해상도와 윈도우 해상도를 일치시킵니다.
	*/
	case EScreenMode::FULLSCREEN_WINDOW:
	{
		ChangeDeviceResolution(m_clientWidth, m_clientHeight);
		break;
	}

	/*
	? -> 전체 화면
	전체 창모드와 같지만 팝업 윈도우로 설정하는 것만 다릅니다.
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
	// 전체 화면일 때만 의미가 있습니다!
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
		// 전체 화면일 때는 클라이언트 영역과 윈도우 영역에 차이가 없습니다!
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
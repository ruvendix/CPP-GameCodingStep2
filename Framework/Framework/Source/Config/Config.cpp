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
	m_deviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
}

void Config::ChangeDeviceResolution(UINT width, UINT height)
{
	m_deviceMode.dmPelsWidth = width;
	m_deviceMode.dmPelsHeight = height;
	::ChangeDisplaySettings(&m_deviceMode, CDS_UPDATEREGISTRY);
}

void Config::AdjustWindowRect(DWORD dwStyle, DWORD dwExStyle)
{
	RECT resultRect;
	::ZeroMemory(&resultRect, sizeof(RECT));
	resultRect.right = GetClientWidth();
	resultRect.bottom = GetClientHeight();

	if (::AdjustWindowRectExForDpi(&resultRect, dwStyle, FALSE, dwExStyle, ::GetDpiForSystem()) == FALSE)
	{
		COM_ERROR(::GetLastError());
	}

	m_windowWidth = resultRect.right - resultRect.left;
	m_windowHeight = resultRect.bottom - resultRect.top;
}
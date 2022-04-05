#include "PCH.h"
#include "Config.h"

#include "PCH.h"
#include "Config.h"

#include "ErrorHandler/ErrorHandler.h"

Config::Config()
{

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
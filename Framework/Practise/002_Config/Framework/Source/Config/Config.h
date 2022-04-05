#pragma once

#include "PCH.h"

class Config
{
	GENERATE_SINGLETON(Config);

public:
	Config();
	~Config() = default;

	UINT GetScreenWidth() const { return m_screenWidth; }
	void SetScreenWidth(UINT screenWidth) { m_screenWidth = screenWidth; }

	UINT GetScreenHeight() const { return m_screenHeight; }
	void SetScreenHeight(UINT screenHeight) { m_screenHeight = screenHeight; }

	UINT GetClientWidth() const { return m_clientWidth; }
	void SetClientWidth(UINT clientWidth) { m_clientWidth = clientWidth; }
			
	UINT GetClientHeight() const { return m_clientHeight; }
	void SetClientHeight(UINT clientHeight) { m_clientHeight = clientHeight; }

	UINT GetWindowWidth() const { return m_windowWidth; }
	UINT GetWindowHeight() const { return m_windowHeight; }

	void AdjustWindowRect(DWORD dwStyle, DWORD dwExStyle);

private:
	UINT m_screenWidth = 0;
	UINT m_screenHeight = 0;

	UINT m_clientWidth = 0;
	UINT m_clientHeight = 0;

	UINT m_windowWidth = 0;
	UINT m_windowHeight = 0;
};
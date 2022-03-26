#pragma once

#include "PCH.h"

enum class EScreenMode : UINT
{
	WINDOW = 0,
	FULLSCREEN,
	FULLSCREEN_WINDOW,
};

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

	const Position2D& GetWindowStartPos() const { return m_windowStartPos; }
	void SetWindowStartPos(UINT x, UINT y) { m_windowStartPos = { x, y }; }
	void SetWindowStartPos(const Position2D& windowStartPos) { m_windowStartPos = windowStartPos; }

	EScreenMode GetCurrentScreenMode() const { return m_currentScreenMode; }
	void SetCurrentScreenMode(EScreenMode currentScreenMode) { m_currentScreenMode = currentScreenMode; }

	EScreenMode GetAltEnterScreenMode() const { return m_altEnterScreenMode; }
	void SetAltEnterScreenMode(EScreenMode altEnterScreenMode) { m_altEnterScreenMode = altEnterScreenMode; }

	const DEVMODE GetDeviceMode() const { return m_deviceMode; }

	bool IsVSYNC() const { return (m_bVSYNC == true); }
	void SetVSYNC(bool bVSYNC) { m_bVSYNC = bVSYNC; }

	void ChangeDeviceResolution(UINT width, UINT height);
	void AdjustWindowRect(DWORD dwStyle, DWORD dwExStyle);

private:
	UINT m_screenWidth = 0;
	UINT m_screenHeight = 0;

	UINT m_clientWidth = 0;
	UINT m_clientHeight = 0;

	UINT m_windowWidth = 0;
	UINT m_windowHeight = 0;

	Position2D m_windowStartPos;

	EScreenMode m_currentScreenMode = EScreenMode::WINDOW;
	EScreenMode m_altEnterScreenMode = EScreenMode::FULLSCREEN; // 기본 전환은 전체 화면

	bool m_bVSYNC = false;

	DEVMODE m_deviceMode;
};
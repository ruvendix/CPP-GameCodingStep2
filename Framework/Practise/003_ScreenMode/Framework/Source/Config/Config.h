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
	~Config();

	void AdjustWindowRect(DWORD dwStyle, DWORD dwExStyle);
	void ChangeDeviceResolution(UINT width, UINT height);

	void AutoChangeScreenMode(HWND hWndViewer);
	void ToggleAltTabState(HWND hWndViewer);

	void CleanUp();

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

	void SetAltEnterScreenMode(EScreenMode altEnterScreenMode) { m_altEnterScreenMode = altEnterScreenMode; }

	bool IsFullScreenMode() const { return (m_currentScreenMode == EScreenMode::FULLSCREEN); }

private:
	Position2D m_windowStartPos;

	UINT m_screenWidth = 0;
	UINT m_screenHeight = 0;

	UINT m_clientWidth = 0;
	UINT m_clientHeight = 0;

	UINT m_windowWidth = 0;
	UINT m_windowHeight = 0;

	bool m_bAltTabMinimize = false;
	EScreenMode m_currentScreenMode = EScreenMode::WINDOW;
	EScreenMode m_altEnterScreenMode = EScreenMode::FULLSCREEN; // 기본 전환은 전체 화면

	DEVMODE m_deviceMode; // 모니터 정보
};
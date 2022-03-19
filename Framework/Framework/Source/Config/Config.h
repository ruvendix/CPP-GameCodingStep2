#pragma once

#include "CommonInclude/Macro.h"

enum class EScreenMode : UINT
{
	WINDOW = 0,
	FULLSCREEN,
	FULLSCREEN_WINDOW,
};

class Config
{
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

	EScreenMode GetCurrentScreenMode() const { return m_currentScreenMode; }
	void SetCurrentScreenMode(EScreenMode currentScreenMode) { m_currentScreenMode = currentScreenMode; }

	EScreenMode GetAltEnterScreenMode() const { return m_altEnterScreenMode; }
	void SetAltEnterScreenMode(EScreenMode altEnterScreenMode) { m_altEnterScreenMode = altEnterScreenMode; }

	const DEVMODE GetDeviceMode() const { return m_deviceMode; }

	bool IsVSYNC() const { return (m_bVSYNC == true); }
	void SetVSYNC(bool bVSYNC) { m_bVSYNC = bVSYNC; }

	void ChangeDeviceResolution(UINT width, UINT height);

private:
	UINT m_screenWidth = 0;
	UINT m_screenHeight = 0;

	UINT m_clientWidth = 0;
	UINT m_clientHeight = 0;

	EScreenMode m_currentScreenMode = EScreenMode::WINDOW;
	EScreenMode m_altEnterScreenMode = EScreenMode::FULLSCREEN; // 기본 전환은 전체 화면

	bool m_bVSYNC = false;

	DEVMODE m_deviceMode;
};
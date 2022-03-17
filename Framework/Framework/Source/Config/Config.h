#pragma once

#include "CommonInclude/Macro.h"

enum class EScreenMode
{
	WINDOW = 0,
	FULLSCREEN,
	FULLSCREEN_WINDOW,
};

class Config
{
public:
	Config() = default;
	~Config() = default;

	UINT GetScreenWidth() const { return m_screenWidth; }
	void SetScreenWidth(UINT screenWidth) { m_screenWidth = screenWidth; }

	UINT GetScreenHeight() const { return m_screenHeight; }
	void SetScreenHeight(UINT screenHeight) { m_screenHeight = screenHeight; }

	EScreenMode GetScreenMode() const { return m_screenMode; }
	void SetScreenMode(EScreenMode screenMode) { m_screenMode = screenMode; }

	bool IsVSYNC() const { return (m_bVSYNC == true); }
	void SetVSYNC(bool bVSYNC) { m_bVSYNC = bVSYNC; }

private:
	UINT m_screenWidth = 0;
	UINT m_screenHeight = 0;
	EScreenMode m_screenMode = EScreenMode::WINDOW;
	bool m_bVSYNC = false;
};
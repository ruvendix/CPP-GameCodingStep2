#pragma once

#include "PCH.h"
#include "CommonInclude/Enum.h"

class WindowViewer;
class WindowProcedure;
class Config;
class Graphics;
class Scene;

class WindowApp
{
public:
	static LRESULT CALLBACK TossWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	WindowApp(HINSTANCE hInst, const std::string& strWndClassName);
	~WindowApp();

	int Run();

	HRESULT StartUp();
	void CleanUp();

	void Do();
	void ToggleScreenMode();
	void ToggleAltTabState(bool bAltTabMinimize);

private:
	HINSTANCE m_hInst = nullptr;

	std::string m_strWndClassName;
	WNDCLASSEX m_wndClass;

	std::shared_ptr<WindowViewer> m_spWndViewer;
	std::shared_ptr<WindowProcedure> m_spWndProcedure;

	std::shared_ptr<Config> m_spConfig;
	std::shared_ptr<Graphics> m_spGfx;

	bool m_bRun = true;

	std::shared_ptr<Scene> m_spScene;
};

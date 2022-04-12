#include "PCH.h"

#include "Config/Config.h"
#include "WindowApp/WindowApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdParam, int cmdShow)
{
	SINGLETON(Config)->SetClientWidth(1600);
	SINGLETON(Config)->SetClientHeight(1200);
	SINGLETON(Config)->SetCurrentScreenMode(EScreenMode::WINDOW);

	std::unique_ptr<WindowApp> spGameApp = std::make_unique<WindowApp>(hInstance, "WindowClass");
	return spGameApp->Run();
}
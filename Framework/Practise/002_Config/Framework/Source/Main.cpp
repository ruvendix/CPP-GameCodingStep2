#include "PCH.h"

#include "WindowApp/WindowApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdParam, int cmdShow)
{
	std::unique_ptr<WindowApp> spGameApp = std::make_unique<WindowApp>(hInstance, "WindowClass");
	return spGameApp->Run();
}
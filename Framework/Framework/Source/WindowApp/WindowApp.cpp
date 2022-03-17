#include "PCH.h"
#include "WindowApp.h"

#include "ErrorHandler/ErrorHandler.h"
#include "WindowViewer/WindowViewer.h"
#include "WindowProcedure/WindowProcedure.h"
#include "Config/Config.h"
#include "Graphics/Graphics.h"

WindowApp::WindowApp(HINSTANCE hInst, const std::string& strWndClassName) :
	m_hInst(hInst),
	m_strWndClassName(strWndClassName)
{
	::ZeroMemory(&m_wndClass, sizeof(WNDCLASSEX));
}

WindowApp::~WindowApp()
{

}

int WindowApp::Run()
{
	if (FAILED(StartUp()))
	{
		return 0;
	}

	MSG msg;
	::ZeroMemory(&msg, sizeof(msg));

	while (m_bRun == true)
	{
		if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		// 탈출 조건
		if ((m_bRun == false) ||
			(msg.message == WM_QUIT))
		{
			CleanUp();
			return msg.wParam;
		}

		Do(); // 사용자가 작성해야 할 코드
	}

	CleanUp();
	return 0;
}

HRESULT WindowApp::StartUp()
{
	m_wndClass.hInstance = m_hInst;
	m_wndClass.cbSize = sizeof(WNDCLASSEX);
	m_wndClass.lpszClassName = m_strWndClassName.c_str();
	m_wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_wndClass.hbrBackground = static_cast<HBRUSH>(::GetStockObject(GRAY_BRUSH));
	m_wndClass.lpfnWndProc = &WindowApp::TossWindowMessage;

	if (::RegisterClassEx(&m_wndClass) == 0)
	{
		return E_FAIL;
	}

	UINT screenWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN));
	UINT screenHeight = static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN));

	m_pConfig = new Config;
	m_pConfig->SetScreenWidth(screenWidth);
	m_pConfig->SetScreenHeight(screenHeight);
	m_pConfig->SetScreenMode(EScreenMode::WINDOW);
	m_pConfig->SetVSYNC(true);

	WindowCreateInfo wndCreateInfo;
	wndCreateInfo.strWndClassName = m_strWndClassName;
	wndCreateInfo.hInst = m_hInst;

	switch (m_pConfig->GetScreenMode())
	{
	case EScreenMode::WINDOW:
	{
		wndCreateInfo.clientWidth = 1600;
		wndCreateInfo.clientHeight = 1200;
		wndCreateInfo.x = static_cast<int>((screenWidth - wndCreateInfo.clientWidth) * 0.5f);
		wndCreateInfo.y = static_cast<int>((screenHeight - wndCreateInfo.clientHeight) * 0.5f);
		wndCreateInfo.dwStyle = WS_OVERLAPPEDWINDOW;

		break;
	}

	case EScreenMode::FULLSCREEN:
	{
		break;
	}

	case EScreenMode::FULLSCREEN_WINDOW:
	{
		ApplyFullScreenWindow();

		wndCreateInfo.clientWidth = screenWidth;
		wndCreateInfo.clientHeight = screenHeight;
		wndCreateInfo.x = 0;
		wndCreateInfo.y = 0;
		wndCreateInfo.dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

		break;
	}
		
	}

	m_pWndProcedure = new WindowProcedure;
	wndCreateInfo.pWndProcedure = m_pWndProcedure;

	m_pWndViewer = new WindowViewer("윈도우 프로그램");
	m_pWndViewer->Create(wndCreateInfo);

	HWND hWnd = m_pWndViewer->GetWindowHandle();
	if (hWnd == nullptr)
	{
		return E_FAIL;
	}

	if (FAILED(SINGLETON(Graphics).StartUp(hWnd, m_pConfig)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void WindowApp::CleanUp()
{
	SINGLETON(Graphics).CleanUp();

	EScreenMode screenMode = m_pConfig->GetScreenMode();
	if (screenMode != EScreenMode::WINDOW)
	{
		::ChangeDisplaySettings(nullptr, 0);
	}

	SAFE_DELETE(m_pConfig);
	SAFE_DELETE(m_pWndProcedure);

	m_pWndViewer->Destroy();
	SAFE_DELETE(m_pWndViewer);

	if (::UnregisterClass(m_strWndClassName.c_str(), m_hInst) == FALSE)
	{
		COM_ERROR(::GetLastError());
	}
}

void WindowApp::Do()
{
	SINGLETON(Graphics).Render();
}

/*
윈도우 메시지를 넘겨주는 함수입니다.
*/
LRESULT WindowApp::TossWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*
	저장한 사용자 정의 정보를 가져옵니다.
	각 객체마다 저장한 정보는 다르고 윈도우 핸들값은 중복되지 않습니다.
	*/
	WindowProcedure* pWndProcedure = nullptr;

	/*
	WM_CREATE 정보에는 전달한 사용자 정의 정보가 있습니다.
	해당 정보를 가져온 후, 메모리에 저장해둡니다.
	*/
	if (msg == WM_CREATE)
	{
		CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		assert(pCreateStruct != nullptr);
		pWndProcedure = reinterpret_cast<WindowProcedure*>(pCreateStruct->lpCreateParams);
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWndProcedure));
	}
	else
	{
		pWndProcedure = reinterpret_cast<WindowProcedure*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

		// WM_CREATE 이전에는 nullptr 고정입니다.
		// 이때는 메시지를 운영체제에게 보냅니다.
		if (pWndProcedure == nullptr)
		{
			// 기본 메시지 핸들러를 이용합니다.
			return ::DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	return pWndProcedure->CallWindowProcedure(hWnd, msg, wParam, lParam);
}

void WindowApp::ApplyFullScreenWindow()
{
	DEVMODE deviceMode;
	::ZeroMemory(&deviceMode, sizeof(DEVMODE));
	deviceMode.dmSize = sizeof(DEVMODE);
	deviceMode.dmPelsWidth = m_pConfig->GetScreenWidth();
	deviceMode.dmPelsHeight = m_pConfig->GetScreenHeight();
	deviceMode.dmBitsPerPel = 32; // 픽셀당 비트수
	deviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	::ChangeDisplaySettings(&deviceMode, CDS_UPDATEREGISTRY);
}
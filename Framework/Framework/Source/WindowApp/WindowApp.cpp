#include "PCH.h"
#include "WindowApp.h"

#include "ErrorHandler/ErrorHandler.h"
#include "WindowViewer/WindowViewer.h"
#include "WindowProcedure/WindowProcedure.h"
#include "Config/Config.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"
#include "InputDevice/InputManager.h"
#include "Scene/Scene.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

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
		COM_ERROR(::GetLastError());
		return E_FAIL;
	}

	UINT screenWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN));
	UINT screenHeight = static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN));

	m_spConfig = std::make_shared<Config>();
	m_spConfig->SetScreenWidth(screenWidth);
	m_spConfig->SetScreenHeight(screenHeight);
	m_spConfig->SetCurrentScreenMode(EScreenMode::WINDOW);
	m_spConfig->SetClientWidth(1600);
	m_spConfig->SetClientHeight(1200);
	m_spConfig->SetVSYNC(true);

	// 전체 화면으로 설정했다면 스왑 모드는 전체 창모드로 바꾼다
	if (m_spConfig->GetCurrentScreenMode() == EScreenMode::FULLSCREEN)
	{
		m_spConfig->SetAltEnterScreenMode(EScreenMode::FULLSCREEN_WINDOW);
	}

	WindowCreateInfo wndCreateInfo;
	wndCreateInfo.strWndClassName = m_strWndClassName;
	wndCreateInfo.hInst = m_hInst;
	wndCreateInfo.dwExStyle = WS_EX_TOPMOST | WS_EX_APPWINDOW;

	EScreenMode screenMode = m_spConfig->GetCurrentScreenMode();
	if (screenMode == EScreenMode::WINDOW)
	{
		wndCreateInfo.dwStyle = WS_OVERLAPPEDWINDOW;
	}
	else
	{
		wndCreateInfo.dwStyle = WS_POPUP;
		m_spConfig->ChangeDeviceResolution(m_spConfig->GetClientWidth(), m_spConfig->GetClientHeight());
	}
	m_spConfig->AdjustWindowRect(wndCreateInfo.dwStyle, wndCreateInfo.dwExStyle);

	wndCreateInfo.windowWidth = m_spConfig->GetWindowWidth();
	wndCreateInfo.windowHeight = m_spConfig->GetWindowHeight();

	wndCreateInfo.windowStartPos.x = static_cast<UINT>((m_spConfig->GetScreenWidth() - wndCreateInfo.windowWidth) * 0.5f);
	wndCreateInfo.windowStartPos.y = static_cast<UINT>((m_spConfig->GetScreenHeight() - wndCreateInfo.windowHeight) * 0.5f);
	m_spConfig->SetWindowStartPos(wndCreateInfo.windowStartPos);

	m_spWndProcedure = std::make_shared<WindowProcedure>();
	m_spWndProcedure->SetConfig(m_spConfig.get());
	m_spWndProcedure->SetWindowApp(this);
	wndCreateInfo.pWndProcedure = m_spWndProcedure.get();

	m_spWndViewer = std::make_shared<WindowViewer>("윈도우 프로그램");
	m_spWndViewer->Create(wndCreateInfo);

	HWND hWnd = m_spWndViewer->GetWindowHandle();
	if (hWnd == nullptr)
	{
		return E_FAIL;
	}

	HRESULT hRet = S_OK;
	TEST_COM(CoInitialize(nullptr), hRet); // COM 사용하기 전에 호출!
	
	m_spGfx = std::make_shared<Graphics>();
	if (FAILED(m_spGfx->StartUp(hWnd, m_spConfig.get())))
	{
		return E_FAIL;
	}

	m_spScene = std::make_shared<Scene>("MainScene", m_spGfx.get());
	m_spScene->StartUp();

	return S_OK;
}

void WindowApp::CleanUp()
{
	m_spScene->CleanUp();
	m_spGfx->CleanUp();

	CoUninitialize(); // COM 사용이 끝났으면 호출!

	if (m_spConfig->GetCurrentScreenMode() != EScreenMode::WINDOW)
	{
		m_spConfig->ChangeDeviceResolution(m_spConfig->GetScreenWidth(), m_spConfig->GetScreenHeight());
	}

	m_spWndViewer->Destroy();

	if (::UnregisterClass(m_strWndClassName.c_str(), m_hInst) == FALSE)
	{
		COM_ERROR(::GetLastError());
	}
}

void WindowApp::Do()
{
	SINGLETON(InputManager).Update();
	m_spScene->Update();

	m_spGfx->BeginRender();
	{
		m_spScene->Render();
	}
	m_spGfx->EndRender();
}

void WindowApp::ToggleScreenMode()
{
	// 현재 화면 모드와 (Alt + Enter) 모드를 바꾸면서 적용
	EScreenMode tempScreenMode = m_spConfig->GetCurrentScreenMode();
	EScreenMode altEnterScreenMode = m_spConfig->GetAltEnterScreenMode();

	m_spConfig->SetCurrentScreenMode(altEnterScreenMode);
	m_spConfig->SetAltEnterScreenMode(tempScreenMode);

	DX11Context* pCtx = m_spGfx->GetContext();
	if (pCtx->GetNativeSwapChain() == nullptr)
	{
		return;
	}

	switch (altEnterScreenMode)
	{
	case EScreenMode::WINDOW: // 모니터 해상도는 윈도우에 설정한 값이지만 창 해상도는 설정했던 값으로 변경하는 경우
	{
		::SetWindowLongPtr(m_spWndViewer->GetWindowHandle(), GWL_STYLE, WS_OVERLAPPEDWINDOW);
		::ShowWindow(m_spWndViewer->GetWindowHandle(), SW_SHOW);

		const Position2D& windowStartPos = m_spConfig->GetWindowStartPos();
		::SetWindowPos(m_spWndViewer->GetWindowHandle(), nullptr, windowStartPos.x, windowStartPos.y, 0, 0, SWP_NOSIZE);

		m_spConfig->ChangeDeviceResolution(m_spConfig->GetScreenWidth(), m_spConfig->GetScreenHeight());
		pCtx->GetNativeSwapChain()->SetFullscreenState(FALSE, nullptr);

		break;
	}

	case EScreenMode::FULLSCREEN_WINDOW: // 모니터 해상도와 창 해상도를 일치시키는 경우
	{
		m_spConfig->ChangeDeviceResolution(m_spConfig->GetClientWidth(), m_spConfig->GetClientHeight());
		pCtx->GetNativeSwapChain()->SetFullscreenState(FALSE, nullptr);

		break;
	}

	case EScreenMode::FULLSCREEN: // 설정한 해상도를 전체 화면으로 설정하는 경우
	{
		m_spConfig->ChangeDeviceResolution(m_spConfig->GetClientWidth(), m_spConfig->GetClientHeight());

		pCtx->RefreshSwapChain();
		pCtx->GetNativeSwapChain()->SetFullscreenState(TRUE, nullptr);

		break;
	}
	}
}

void WindowApp::ToggleAltTabState(bool bAltTabMinimize)
{
	EScreenMode screenMode = m_spConfig->GetCurrentScreenMode();
	if (screenMode != EScreenMode::FULLSCREEN)
	{
		return;
	}

	if (bAltTabMinimize == true)
	{
		::SetWindowLongPtr(m_spWndViewer->GetWindowHandle(), GWL_STYLE, WS_MINIMIZE);
		::ShowWindow(m_spWndViewer->GetWindowHandle(), SW_SHOW);
	}
	else
	{
		::SetWindowLongPtr(m_spWndViewer->GetWindowHandle(), GWL_STYLE, WS_POPUP);
		::ShowWindow(m_spWndViewer->GetWindowHandle(), SW_SHOW);

		m_spConfig->ChangeDeviceResolution(m_spConfig->GetClientWidth(), m_spConfig->GetClientHeight());

		DX11Context* pCtx = m_spGfx->GetContext();
		ASSERT(pCtx != nullptr);
		IDXGISwapChain* pSwapChain = pCtx->GetNativeSwapChain();
		ASSERT(pSwapChain != nullptr);
		pSwapChain->SetFullscreenState(TRUE, nullptr);
	}
}
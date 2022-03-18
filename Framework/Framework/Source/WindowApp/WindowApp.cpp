#include "PCH.h"
#include "WindowApp.h"

#include "ErrorHandler/ErrorHandler.h"
#include "WindowViewer/WindowViewer.h"
#include "WindowProcedure/WindowProcedure.h"
#include "Config/Config.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

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
		return E_FAIL;
	}

	UINT screenWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN));
	UINT screenHeight = static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN));

	m_pConfig = new Config;
	m_pConfig->SetScreenWidth(screenWidth);
	m_pConfig->SetScreenHeight(screenHeight);
	m_pConfig->SetCurrentScreenMode(EScreenMode::FULLSCREEN_WINDOW);
	m_pConfig->SetClientWidth(1600);
	m_pConfig->SetClientHeight(1200);
	m_pConfig->SetVSYNC(true);

	// 전체 화면으로 설정했다면 스왑 모드는 전체 창모드로 바꾼다
	if (m_pConfig->GetCurrentScreenMode() == EScreenMode::FULLSCREEN)
	{
		m_pConfig->SetSwapScreenMode(EScreenMode::FULLSCREEN_WINDOW);
	}

	WindowCreateInfo wndCreateInfo;
	wndCreateInfo.clientWidth = m_pConfig->GetClientWidth();
	wndCreateInfo.clientHeight = m_pConfig->GetClientHeight();
	wndCreateInfo.strWndClassName = m_strWndClassName;
	wndCreateInfo.hInst = m_hInst;

	switch (m_pConfig->GetCurrentScreenMode())
	{
	case EScreenMode::WINDOW:
	{
		wndCreateInfo.x = static_cast<int>((screenWidth - wndCreateInfo.clientWidth) * 0.5f);
		wndCreateInfo.y = static_cast<int>((screenHeight - wndCreateInfo.clientHeight) * 0.5f);
		wndCreateInfo.dwStyle = WS_OVERLAPPEDWINDOW;

		break;
	}

	case EScreenMode::FULLSCREEN:
	case EScreenMode::FULLSCREEN_WINDOW:
	{
		wndCreateInfo.x = 0;
		wndCreateInfo.y = 0;
		wndCreateInfo.dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

		m_pConfig->ChangeDeviceResolution(wndCreateInfo.clientWidth, wndCreateInfo.clientHeight);
		break;
	}
		
	}

	m_pWndProcedure = new WindowProcedure;
	m_pWndProcedure->SetConfig(m_pConfig);
	m_pWndProcedure->SetWindowApp(this);
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

	if (m_pConfig->GetCurrentScreenMode() != EScreenMode::WINDOW)
	{
		m_pConfig->ChangeDeviceResolution(m_pConfig->GetScreenWidth(), m_pConfig->GetScreenHeight());
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

void WindowApp::ToggleScreenMode()
{
	// 현재 화면 모드와 스왑할 모드를 바꾸면서 적용
	EScreenMode tempScreenMode = m_pConfig->GetCurrentScreenMode();
	EScreenMode swapScreenMode = m_pConfig->GetSwapScreenMode();

	m_pConfig->SetCurrentScreenMode(swapScreenMode);
	m_pConfig->SetSwapScreenMode(tempScreenMode);

	DX11Context* pCtx = SINGLETON(Graphics).GetContext();
	if (pCtx->GetSwapChain() == nullptr)
	{
		return;
	}

	switch (swapScreenMode)
	{
	case EScreenMode::WINDOW: // 모니터 해상도는 윈도우에 설정한 값이지만 창 해상도는 설정했던 값으로 변경하는 경우
	{
		::SetWindowLong(m_pWndViewer->GetWindowHandle(), GWL_STYLE, WS_OVERLAPPEDWINDOW);
		::ShowWindow(m_pWndViewer->GetWindowHandle(), SW_SHOW);

		m_pConfig->ChangeDeviceResolution(m_pConfig->GetScreenWidth(), m_pConfig->GetScreenHeight());
		pCtx->GetSwapChain()->SetFullscreenState(FALSE, nullptr);

		break;
	}

	case EScreenMode::FULLSCREEN_WINDOW: // 모니터 해상도와 창 해상도를 일치시키는 경우
	{
		m_pConfig->ChangeDeviceResolution(m_pConfig->GetClientWidth(), m_pConfig->GetClientHeight());
		pCtx->GetSwapChain()->SetFullscreenState(FALSE, nullptr);

		break;
	}

	case EScreenMode::FULLSCREEN: // 설정한 해상도를 전체 화면으로 설정하는 경우
	{
		m_pConfig->ChangeDeviceResolution(m_pConfig->GetClientWidth(), m_pConfig->GetClientHeight());
		pCtx->GetSwapChain()->SetFullscreenState(TRUE, nullptr);

		break;
	}
	}
}

void WindowApp::ToggleAltTabState(EAltTabState altTabState)
{
	EScreenMode screenMode = m_pConfig->GetCurrentScreenMode();
	if (screenMode != EScreenMode::FULLSCREEN)
	{
		return;
	}

	if (altTabState == EAltTabState::MINIMIZE)
	{
		::OutputDebugString("최소화시킴\n");
		::SetWindowLong(m_pWndViewer->GetWindowHandle(), GWL_STYLE, WS_MINIMIZE);
		::ShowWindow(m_pWndViewer->GetWindowHandle(), SW_SHOW);
	}
	else
	{
		::SetWindowLong(m_pWndViewer->GetWindowHandle(), GWL_STYLE, WS_POPUP);
		::ShowWindow(m_pWndViewer->GetWindowHandle(), SW_SHOW);

		m_pConfig->ChangeDeviceResolution(m_pConfig->GetClientWidth(), m_pConfig->GetClientHeight());

		DX11Context* pCtx = SINGLETON(Graphics).GetContext();
		if (pCtx->GetSwapChain() == nullptr)
		{
			return;
		}
		pCtx->GetSwapChain()->SetFullscreenState(TRUE, nullptr);
	}
}
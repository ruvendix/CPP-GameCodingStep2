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
������ �޽����� �Ѱ��ִ� �Լ��Դϴ�.
*/
LRESULT WindowApp::TossWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*
	������ ����� ���� ������ �����ɴϴ�.
	�� ��ü���� ������ ������ �ٸ��� ������ �ڵ鰪�� �ߺ����� �ʽ��ϴ�.
	*/
	WindowProcedure* pWndProcedure = nullptr;

	/*
	WM_CREATE �������� ������ ����� ���� ������ �ֽ��ϴ�.
	�ش� ������ ������ ��, �޸𸮿� �����صӴϴ�.
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

		// WM_CREATE �������� nullptr �����Դϴ�.
		// �̶��� �޽����� �ü������ �����ϴ�.
		if (pWndProcedure == nullptr)
		{
			// �⺻ �޽��� �ڵ鷯�� �̿��մϴ�.
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

		// Ż�� ����
		if ((m_bRun == false) ||
			(msg.message == WM_QUIT))
		{
			CleanUp();
			return msg.wParam;
		}

		Do(); // ����ڰ� �ۼ��ؾ� �� �ڵ�
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

	// ��ü ȭ������ �����ߴٸ� ���� ���� ��ü â���� �ٲ۴�
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

	m_pWndViewer = new WindowViewer("������ ���α׷�");
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
	// ���� ȭ�� ���� ������ ��带 �ٲٸ鼭 ����
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
	case EScreenMode::WINDOW: // ����� �ػ󵵴� �����쿡 ������ �������� â �ػ󵵴� �����ߴ� ������ �����ϴ� ���
	{
		::SetWindowLong(m_pWndViewer->GetWindowHandle(), GWL_STYLE, WS_OVERLAPPEDWINDOW);
		::ShowWindow(m_pWndViewer->GetWindowHandle(), SW_SHOW);

		m_pConfig->ChangeDeviceResolution(m_pConfig->GetScreenWidth(), m_pConfig->GetScreenHeight());
		pCtx->GetSwapChain()->SetFullscreenState(FALSE, nullptr);

		break;
	}

	case EScreenMode::FULLSCREEN_WINDOW: // ����� �ػ󵵿� â �ػ󵵸� ��ġ��Ű�� ���
	{
		m_pConfig->ChangeDeviceResolution(m_pConfig->GetClientWidth(), m_pConfig->GetClientHeight());
		pCtx->GetSwapChain()->SetFullscreenState(FALSE, nullptr);

		break;
	}

	case EScreenMode::FULLSCREEN: // ������ �ػ󵵸� ��ü ȭ������ �����ϴ� ���
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
		::OutputDebugString("�ּ�ȭ��Ŵ\n");
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
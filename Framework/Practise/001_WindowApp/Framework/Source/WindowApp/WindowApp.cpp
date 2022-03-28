#include "PCH.h"
#include "WindowApp.h"

#include "ErrorHandler/ErrorHandler.h"
#include "WindowViewer/WindowViewer.h"
#include "WindowProcedure/WindowProcedure.h"

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

		/*
		WM_CREATE 이전에는 nullptr 고정입니다.
		이때는 메시지를 운영체제에게 보냅니다.
		*/
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

	WindowCreateInfo wndCreateInfo;
	wndCreateInfo.strWndClassName = m_strWndClassName;
	wndCreateInfo.hInst = m_hInst;
	wndCreateInfo.dwExStyle = WS_EX_TOPMOST | WS_EX_APPWINDOW;
	wndCreateInfo.dwStyle = WS_OVERLAPPEDWINDOW;
	wndCreateInfo.windowWidth = 1600;
	wndCreateInfo.windowHeight = 1200;

	UINT screenWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN));
	wndCreateInfo.windowStartPos.x = static_cast<UINT>((screenWidth - wndCreateInfo.windowWidth) * 0.5f);

	UINT screenHeight = static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN));
	wndCreateInfo.windowStartPos.y = static_cast<UINT>((screenHeight - wndCreateInfo.windowHeight) * 0.5f);

	m_spWndProcedure = std::make_shared<WindowProcedure>();
	wndCreateInfo.pWndProcedure = m_spWndProcedure.get();

	m_spWndViewer = std::make_shared<WindowViewer>("WindowApp");
	m_spWndViewer->Create(wndCreateInfo);

	HWND hWnd = m_spWndViewer->GetWindowHandle();
	if (hWnd == nullptr)
	{
		return E_FAIL;
	}
	
	return S_OK;
}

void WindowApp::CleanUp()
{
	m_spWndViewer->Destroy();

	if (::UnregisterClass(m_strWndClassName.c_str(), m_hInst) == FALSE)
	{
		COM_ERROR(::GetLastError());
	}
}

void WindowApp::Do()
{

}
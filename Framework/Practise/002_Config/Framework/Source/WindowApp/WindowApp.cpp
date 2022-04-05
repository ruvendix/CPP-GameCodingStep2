#include "PCH.h"
#include "WindowApp.h"

#include "ErrorHandler/ErrorHandler.h"
#include "WindowViewer/WindowViewer.h"
#include "WindowProcedure/WindowProcedure.h"
#include "Config/Config.h"

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

		/*
		WM_CREATE �������� nullptr �����Դϴ�.
		�̶��� �޽����� �ü������ �����ϴ�.
		*/
		if (pWndProcedure == nullptr)
		{
			// �⺻ �޽��� ���ν����� �̿��մϴ�.
			return ::DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	return pWndProcedure->CallWindowProcedure(hWnd, msg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		COM_ERROR(::GetLastError());
		return E_FAIL;
	}

	WindowViewerDescription wndViewerDesc;
	wndViewerDesc.strWndClassName = m_strWndClassName;
	wndViewerDesc.hInst = m_hInst;
	wndViewerDesc.dwExStyle = WS_EX_APPWINDOW;
	wndViewerDesc.dwStyle = WS_OVERLAPPEDWINDOW;

	SINGLETON(Config)->SetClientWidth(1600);
	SINGLETON(Config)->SetClientHeight(1200);
	SINGLETON(Config)->AdjustWindowRect(wndViewerDesc.dwStyle, wndViewerDesc.dwExStyle);

	wndViewerDesc.wndWidth = SINGLETON(Config)->GetWindowWidth();
	wndViewerDesc.wndHeight = SINGLETON(Config)->GetWindowHeight();

	UINT screenWidth = static_cast<UINT>(::GetSystemMetrics(SM_CXSCREEN));
	wndViewerDesc.wndStartPos.x = static_cast<UINT>((screenWidth - wndViewerDesc.wndWidth) * 0.5f);

	UINT screenHeight = static_cast<UINT>(::GetSystemMetrics(SM_CYSCREEN));
	wndViewerDesc.wndStartPos.y = static_cast<UINT>((screenHeight - wndViewerDesc.wndHeight) * 0.5f);

	m_spWndProcedure = std::make_shared<WindowProcedure>();
	wndViewerDesc.pWndProcedure = m_spWndProcedure.get();

	m_spWndViewer = std::make_shared<WindowViewer>("WindowApp");
	m_spWndViewer->Create(wndViewerDesc);

	return S_OK;
}

void WindowApp::Do()
{

}

void WindowApp::CleanUp()
{
	m_spWndViewer->Destroy();

	if (::UnregisterClass(m_strWndClassName.c_str(), m_hInst) == FALSE)
	{
		COM_ERROR(::GetLastError());
	}
}
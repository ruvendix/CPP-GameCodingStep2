#include "PCH.h"
#include "DX11Context.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Config/Config.h"
#include "Graphics/Graphics.h"

DX11Context::DX11Context(Graphics* pGFX, Config* pConfig)
{
	m_pGFX = pGFX;
	m_pConfig = pConfig;

	//m_clearColor = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	::ZeroMemory(&m_clearColor, sizeof(DirectX::XMFLOAT4));
}

HRESULT DX11Context::StartUp(HWND hWnd)
{
	HRESULT hRet = S_OK;

	// 픽셀당 4바이트(32비트)를 사용하는 게 일반적이고
	// 각 색상값을 어떻게 표현하는지가 중요함!
	// SNORM    => signed normalization, -1 ~ 1
	// UNORM    => unsinged normalization, 0 ~ 1, 웬만해서는 그냥 이거
	// INT~     => -(2^(색상값당 비트수 - 1)) ~ (2^(색상값당 비트수 - 1)) - 1
	// UINT~    => 0 ~ (2^(색상값당 비트수))
	// FLOAT    => 4바이트 실수값(값 저장용으로 많이 사용됨)
	// TYPELESS => 해석 방법을 정하지 않음(값 자체를 전달할 때 사용됨)
	DXGI_FORMAT renderingFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

#pragma region 모니터와 그래픽 카드 호환성 검사 및 정보 확인
	// Factory 패턴이 생각나는 코드
	IDXGIFactory* pFactory = nullptr;
	TEST_COM(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory)), hRet);

	// 첫번째 그래픽 카드에 대한 어댑터를 가져옵니다. (인자로 0을 넣음)
	// 어댑터 자체를 그래픽 카드라고 봐도 무난합니다.
	IDXGIAdapter* pAdapter = nullptr;
	TEST_COM(pFactory->EnumAdapters(0, &pAdapter), hRet);

	// 첫번째 어댑터에 대한 출력 장치를 가져옵니다. (모니터 얘기)
	IDXGIOutput* pAdapterOutput;
	TEST_COM(pAdapter->EnumOutputs(0, &pAdapterOutput), hRet);

	// DXGI_FORMAT_R8G8B8A8_UNORM에 해당되는 모드 개수를 가져옵니다.
	UINT numMode = 0;
	TEST_COM(pAdapterOutput->GetDisplayModeList(renderingFormat,
		DXGI_ENUM_MODES_INTERLACED, &numMode, nullptr), hRet);

	std::vector<DXGI_MODE_DESC> vecMode(numMode);
	TEST_COM(pAdapterOutput->GetDisplayModeList(renderingFormat,
		DXGI_ENUM_MODES_INTERLACED, &numMode, vecMode.data()), hRet);

	UINT refreshRateNumerator = 0;
	UINT refreshRateDenominator = 0;
	
	// 가져온 모드와 그래픽 카드가 호환되는지 검사합니다.
	// 최종적으로 갱신된 값만 사용합니다.
	for (UINT i = 0; i < numMode; ++i)
	{
		if ((vecMode[i].Width == m_pConfig->GetScreenWidth()) &&
			(vecMode[i].Height == m_pConfig->GetScreenHeight()))
		{
			refreshRateNumerator = vecMode[i].RefreshRate.Numerator; // 분자
			refreshRateDenominator = vecMode[i].RefreshRate.Denominator; // 분모
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// 여기부터 그래픽 카드 정보 확인하는 부분
	
	// 어댑터 정보를 가져옵니다.
	DXGI_ADAPTER_DESC adapterDesc;
	::ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));
	TEST_COM(pAdapter->GetDesc(&adapterDesc), hRet);

	// 어댑터의 VRAM을 MB단위로 확인합니다.
	SIZE_T VRAM_size = ((adapterDesc.DedicatedVideoMemory / 1024) / 1024);
	m_pGFX->SetVRAM_Size(VRAM_size);

	// 어댑터의 이름을 확인합니다.
	WSTRING_TO_STRING(strGraphicCardName, adapterDesc.Description);
	m_pGFX->SetGraphicCardName(strGraphicCardName);

	SAFE_RELEASE(pAdapterOutput);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);
#pragma endregion

#pragma region 스왑체인 초기화
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	::ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1; // 백버퍼 사용 개수(프론트 버퍼 포함 2개)
	swapChainDesc.BufferDesc.Width = m_pConfig->GetScreenWidth(); // 백버퍼 너비
	swapChainDesc.BufferDesc.Height = m_pConfig->GetScreenHeight(); // 백퍼버 높이
	swapChainDesc.BufferDesc.Format = renderingFormat;

	if (m_pConfig->IsVSYNC())
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRateNumerator; // 분자
		swapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRateDenominator; // 분모
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0; // 분자
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // 분모
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 백퍼버 용도
	swapChainDesc.OutputWindow = hWnd; // 렌더링할 윈도우 핸들
	swapChainDesc.SampleDesc.Count = 1; // 픽셀당 MSAA 개수
	swapChainDesc.SampleDesc.Quality = 0; // MSAA 수준

	// 전체 화면일 때만 구분합니다.
	// 창모드와 전체 창모드는 결국 창모드입니다.
	EScreenMode screenMode = m_pConfig->GetScreenMode();
	if (screenMode == EScreenMode::FULLSCREEN)
	{
		swapChainDesc.Windowed = FALSE;
	}
	else
	{
		swapChainDesc.Windowed = TRUE;
	}

	// 스캔라인 정렬 순서와 확대 기능은 지정되지 않음으로 설정합니다.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 출력된 백버퍼 내용을 버리도록 설정합니다.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#pragma endregion

#pragma region 디바이스, 스왑체인, 렌더타겟뷰 생성
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0; // DX11을 사용

	TEST_COM(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pDeviceCtx), hRet);

	// 생성한 백버퍼를 가져옵니다. (간단히 말하면 이미지, 즉 텍스처)
	ID3D11Texture2D* pBackBufferTex = nullptr;
	TEST_COM(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&pBackBufferTex)), hRet);

	// 백버퍼를 렌더타겟뷰에 연결합니다.
	// 렌더링된 결과를 볼 수 있게 하는 작업이라고 보면 됩니다.
	TEST_COM(m_pDevice->CreateRenderTargetView(pBackBufferTex, nullptr, &m_pRenderTargetView), hRet);

	// 백버퍼를 연결시켰으니 이제 필요 없습니다.
	SAFE_RELEASE(pBackBufferTex);

	// 렌더링할 렌더타겟뷰를 연결합니다.
	m_pDeviceCtx->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
#pragma endregion

	return S_OK;
}

void DX11Context::CleanUp()
{
	// 종료하기 전에는 항상 창모드로 변경해야 안전하게 해제할 수 있습니다.
	m_pSwapChain->SetFullscreenState(FALSE, nullptr);
	
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceCtx);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pSwapChain);
}

void DX11Context::BeginRender()
{
	static std::default_random_engine randomEngine;
	std::uniform_real_distribution<FLOAT> randNum(0.0f);

	static DWORD dwLocalTime = ::timeGetTime();
	static DWORD elapsedTime = 0;
	elapsedTime += (::timeGetTime() - dwLocalTime);
	dwLocalTime = ::timeGetTime();

	if (elapsedTime > 1000)
	{
		elapsedTime = 0;

		m_clearColor.x += randNum(randomEngine);
		if (m_clearColor.x > 1.0f)
		{
			m_clearColor.x = 0.0f;
		}

		m_clearColor.y += randNum(randomEngine);
		if (m_clearColor.y > 1.0f)
		{
			m_clearColor.y = 0.0f;
		}

		m_clearColor.z += randNum(randomEngine);
		if (m_clearColor.z > 1.0f)
		{
			m_clearColor.z = 0.0f;
		}
	}

	m_pDeviceCtx->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<FLOAT*>(&m_clearColor));
}

void DX11Context::EndRender()
{
	if (m_pConfig->IsVSYNC())
	{
		m_pSwapChain->Present(1, 0); // 갱신 비율을 고정합니다.
	}
	else
	{
		m_pSwapChain->Present(0, 0); // 최대한 빠르게 갱신합니다.
	}
}
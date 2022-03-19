#include "PCH.h"
#include "DX11Context.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Config/Config.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Adapter.h"

class DX11ContextInside
{
	INSIDE_CLASS(DX11Context, DX11ContextInside);

public:
	void InitSwapChainDesc(HWND hWnd, const DXGI_RATIONAL& refreshRate, DXGI_SWAP_CHAIN_DESC& refSwapChainDesc);
};

void DX11ContextInside::InitSwapChainDesc(HWND hWnd, const DXGI_RATIONAL& refreshRate, DXGI_SWAP_CHAIN_DESC& refSwapChainDesc)
{
	refSwapChainDesc.BufferCount = 1; // 백버퍼 사용 개수(프론트 버퍼 포함 2개)

	const Config* pConfig = m_pOutside->m_pConfig;
	refSwapChainDesc.BufferDesc.Width = pConfig->GetClientWidth(); // 백버퍼 너비
	refSwapChainDesc.BufferDesc.Height = pConfig->GetClientHeight(); // 백퍼버 높이

	// 픽셀당 4바이트(32비트)를 사용하는 게 일반적이고
	// 각 색상값을 어떻게 표현하는지가 중요함!
	// SNORM    => signed normalization, -1 ~ 1
	// UNORM    => unsinged normalization, 0 ~ 1, 웬만해서는 그냥 이거
	// INT~     => -(2^(색상값당 비트수 - 1)) ~ (2^(색상값당 비트수 - 1)) - 1
	// UINT~    => 0 ~ (2^(색상값당 비트수))
	// FLOAT    => 4바이트 실수값(값 저장용으로 많이 사용됨)
	// TYPELESS => 해석 방법을 정하지 않음(값 자체를 전달할 때 사용됨)
	refSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (pConfig->IsVSYNC())
	{
		refSwapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRate.Numerator; // 분자
		refSwapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRate.Denominator; // 분모
	}
	else
	{
		refSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0; // 분자
		refSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // 분모
	}

	refSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 백퍼버 용도
	refSwapChainDesc.OutputWindow = hWnd; // 렌더링할 윈도우 핸들
	refSwapChainDesc.SampleDesc.Count = 1; // 픽셀당 MSAA 개수

	// 전체 화면일 때만 구분합니다.
	// 창모드와 전체 창모드는 결국 창모드입니다.
	EScreenMode screenMode = pConfig->GetCurrentScreenMode();
	if (screenMode == EScreenMode::FULLSCREEN)
	{
		refSwapChainDesc.Windowed = FALSE;
	}
	else
	{
		refSwapChainDesc.Windowed = TRUE;
	}
}

///////////////////////////////////////////////////////////////////
DX11Context::DX11Context(Graphics* pGFX, Config* pConfig)
{
	m_pGFX = pGFX;
	m_pConfig = pConfig;

	//m_clearColor = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	::ZeroMemory(&m_clearColor, sizeof(DirectX::XMFLOAT4));

	m_spInside = std::make_shared<DX11ContextInside>(this);
}

HRESULT DX11Context::StartUp(HWND hWnd)
{
	HRESULT hRet = S_OK;

	// 어댑터 정보를 가져옵니다.
	m_spAdpater = std::make_shared<DX11Adapter>();
	TEST_COM(m_spAdpater->StartUp(), hRet);

	// 어댑터 정보와 호환되는 주사율 갱신 비율을 알아냅니다.
	DXGI_RATIONAL refreshRate;
	m_spAdpater->FindValidRefeshRate(m_pConfig->GetClientWidth(), m_pConfig->GetClientHeight(), refreshRate);

	// 스왑체인 정보를 설정합니다.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	::ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	m_spInside->InitSwapChainDesc(hWnd, refreshRate, swapChainDesc);

	// 설정한 정보로 스왑체인, 디바이스, 디바이스 컨텍스트를 생성합니다.
	TEST_COM(D3D11CreateDeviceAndSwapChain(
		nullptr, // 기본 어댑터 사용
		D3D_DRIVER_TYPE_HARDWARE, // 하드웨어 사용
		nullptr, // 하드웨어를 사용하므로 무시
		0, // 플래그 무시
		nullptr, // 피처레벨은 알아서 정함
		0, // 피처레벨 개수는 0
		D3D11_SDK_VERSION, // 이 값으로 고정
		&swapChainDesc, // 설정한 스왑체인 정보
		m_spSwapChain.GetAddressOf(), // 생성할 스왑체인
		m_spDevice.GetAddressOf(), // 생성할 디바이스
		nullptr, // 사용하지 않으므로 무시
		m_spDeviceCtx.GetAddressOf()), // 생성할 디바이스 컨텍스트
		hRet);

	// 생성한 백버퍼를 가져옵니다. (간단히 말하면 이미지, 즉 텍스처)
	Microsoft::WRL::ComPtr<ID3D11Texture2D> spBackBufferTex;
	TEST_COM(m_spSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(spBackBufferTex.GetAddressOf())), hRet);

	// 백버퍼를 렌더타겟뷰에 연결합니다.
	// 렌더링된 결과를 볼 수 있게 하는 작업이라고 보면 됩니다.
	TEST_COM(m_spDevice->CreateRenderTargetView(spBackBufferTex.Get(), nullptr, m_spRenderTargetView.GetAddressOf()), hRet);

	// 렌더링할 렌더타겟뷰를 연결합니다.
	m_spDeviceCtx->OMSetRenderTargets(1, m_spRenderTargetView.GetAddressOf(), nullptr);
#pragma endregion

	return S_OK;
}

void DX11Context::CleanUp()
{
	// 종료하기 전에는 항상 창모드로 변경해야 안전하게 해제할 수 있습니다.
	if (m_pConfig->GetCurrentScreenMode() == EScreenMode::FULLSCREEN)
	{
		m_spSwapChain->SetFullscreenState(FALSE, nullptr);
	}
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

	m_spDeviceCtx->ClearRenderTargetView(m_spRenderTargetView.Get(), reinterpret_cast<FLOAT*>(&m_clearColor));
}

void DX11Context::EndRender()
{
	if (m_pConfig->IsVSYNC())
	{
		m_spSwapChain->Present(1, 0); // 갱신 비율을 고정합니다.
	}
	else
	{
		m_spSwapChain->Present(0, 0); // 최대한 빠르게 갱신합니다.
	}
}
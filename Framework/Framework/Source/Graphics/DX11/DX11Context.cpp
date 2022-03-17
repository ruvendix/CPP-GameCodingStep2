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

	// �ȼ��� 4����Ʈ(32��Ʈ)�� ����ϴ� �� �Ϲ����̰�
	// �� ������ ��� ǥ���ϴ����� �߿���!
	// SNORM    => signed normalization, -1 ~ 1
	// UNORM    => unsinged normalization, 0 ~ 1, �����ؼ��� �׳� �̰�
	// INT~     => -(2^(���󰪴� ��Ʈ�� - 1)) ~ (2^(���󰪴� ��Ʈ�� - 1)) - 1
	// UINT~    => 0 ~ (2^(���󰪴� ��Ʈ��))
	// FLOAT    => 4����Ʈ �Ǽ���(�� ��������� ���� ����)
	// TYPELESS => �ؼ� ����� ������ ����(�� ��ü�� ������ �� ����)
	DXGI_FORMAT renderingFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

#pragma region ����Ϳ� �׷��� ī�� ȣȯ�� �˻� �� ���� Ȯ��
	// Factory ������ �������� �ڵ�
	IDXGIFactory* pFactory = nullptr;
	TEST_COM(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory)), hRet);

	// ù��° �׷��� ī�忡 ���� ����͸� �����ɴϴ�. (���ڷ� 0�� ����)
	// ����� ��ü�� �׷��� ī���� ���� �����մϴ�.
	IDXGIAdapter* pAdapter = nullptr;
	TEST_COM(pFactory->EnumAdapters(0, &pAdapter), hRet);

	// ù��° ����Ϳ� ���� ��� ��ġ�� �����ɴϴ�. (����� ���)
	IDXGIOutput* pAdapterOutput;
	TEST_COM(pAdapter->EnumOutputs(0, &pAdapterOutput), hRet);

	// DXGI_FORMAT_R8G8B8A8_UNORM�� �ش�Ǵ� ��� ������ �����ɴϴ�.
	UINT numMode = 0;
	TEST_COM(pAdapterOutput->GetDisplayModeList(renderingFormat,
		DXGI_ENUM_MODES_INTERLACED, &numMode, nullptr), hRet);

	std::vector<DXGI_MODE_DESC> vecMode(numMode);
	TEST_COM(pAdapterOutput->GetDisplayModeList(renderingFormat,
		DXGI_ENUM_MODES_INTERLACED, &numMode, vecMode.data()), hRet);

	UINT refreshRateNumerator = 0;
	UINT refreshRateDenominator = 0;
	
	// ������ ���� �׷��� ī�尡 ȣȯ�Ǵ��� �˻��մϴ�.
	// ���������� ���ŵ� ���� ����մϴ�.
	for (UINT i = 0; i < numMode; ++i)
	{
		if ((vecMode[i].Width == m_pConfig->GetScreenWidth()) &&
			(vecMode[i].Height == m_pConfig->GetScreenHeight()))
		{
			refreshRateNumerator = vecMode[i].RefreshRate.Numerator; // ����
			refreshRateDenominator = vecMode[i].RefreshRate.Denominator; // �и�
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	// ������� �׷��� ī�� ���� Ȯ���ϴ� �κ�
	
	// ����� ������ �����ɴϴ�.
	DXGI_ADAPTER_DESC adapterDesc;
	::ZeroMemory(&adapterDesc, sizeof(DXGI_ADAPTER_DESC));
	TEST_COM(pAdapter->GetDesc(&adapterDesc), hRet);

	// ������� VRAM�� MB������ Ȯ���մϴ�.
	SIZE_T VRAM_size = ((adapterDesc.DedicatedVideoMemory / 1024) / 1024);
	m_pGFX->SetVRAM_Size(VRAM_size);

	// ������� �̸��� Ȯ���մϴ�.
	WSTRING_TO_STRING(strGraphicCardName, adapterDesc.Description);
	m_pGFX->SetGraphicCardName(strGraphicCardName);

	SAFE_RELEASE(pAdapterOutput);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);
#pragma endregion

#pragma region ����ü�� �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	::ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1; // ����� ��� ����(����Ʈ ���� ���� 2��)
	swapChainDesc.BufferDesc.Width = m_pConfig->GetScreenWidth(); // ����� �ʺ�
	swapChainDesc.BufferDesc.Height = m_pConfig->GetScreenHeight(); // ���۹� ����
	swapChainDesc.BufferDesc.Format = renderingFormat;

	if (m_pConfig->IsVSYNC())
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRateNumerator; // ����
		swapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRateDenominator; // �и�
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0; // ����
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // �и�
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���۹� �뵵
	swapChainDesc.OutputWindow = hWnd; // �������� ������ �ڵ�
	swapChainDesc.SampleDesc.Count = 1; // �ȼ��� MSAA ����
	swapChainDesc.SampleDesc.Quality = 0; // MSAA ����

	// ��ü ȭ���� ���� �����մϴ�.
	// â���� ��ü â���� �ᱹ â����Դϴ�.
	EScreenMode screenMode = m_pConfig->GetScreenMode();
	if (screenMode == EScreenMode::FULLSCREEN)
	{
		swapChainDesc.Windowed = FALSE;
	}
	else
	{
		swapChainDesc.Windowed = TRUE;
	}

	// ��ĵ���� ���� ������ Ȯ�� ����� �������� �������� �����մϴ�.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ��µ� ����� ������ �������� �����մϴ�.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#pragma endregion

#pragma region ����̽�, ����ü��, ����Ÿ�ٺ� ����
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0; // DX11�� ���

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

	// ������ ����۸� �����ɴϴ�. (������ ���ϸ� �̹���, �� �ؽ�ó)
	ID3D11Texture2D* pBackBufferTex = nullptr;
	TEST_COM(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&pBackBufferTex)), hRet);

	// ����۸� ����Ÿ�ٺ信 �����մϴ�.
	// �������� ����� �� �� �ְ� �ϴ� �۾��̶�� ���� �˴ϴ�.
	TEST_COM(m_pDevice->CreateRenderTargetView(pBackBufferTex, nullptr, &m_pRenderTargetView), hRet);

	// ����۸� ����������� ���� �ʿ� �����ϴ�.
	SAFE_RELEASE(pBackBufferTex);

	// �������� ����Ÿ�ٺ並 �����մϴ�.
	m_pDeviceCtx->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
#pragma endregion

	return S_OK;
}

void DX11Context::CleanUp()
{
	// �����ϱ� ������ �׻� â���� �����ؾ� �����ϰ� ������ �� �ֽ��ϴ�.
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
		m_pSwapChain->Present(1, 0); // ���� ������ �����մϴ�.
	}
	else
	{
		m_pSwapChain->Present(0, 0); // �ִ��� ������ �����մϴ�.
	}
}
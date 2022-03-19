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
	refSwapChainDesc.BufferCount = 1; // ����� ��� ����(����Ʈ ���� ���� 2��)

	const Config* pConfig = m_pOutside->m_pConfig;
	refSwapChainDesc.BufferDesc.Width = pConfig->GetClientWidth(); // ����� �ʺ�
	refSwapChainDesc.BufferDesc.Height = pConfig->GetClientHeight(); // ���۹� ����

	// �ȼ��� 4����Ʈ(32��Ʈ)�� ����ϴ� �� �Ϲ����̰�
	// �� ������ ��� ǥ���ϴ����� �߿���!
	// SNORM    => signed normalization, -1 ~ 1
	// UNORM    => unsinged normalization, 0 ~ 1, �����ؼ��� �׳� �̰�
	// INT~     => -(2^(���󰪴� ��Ʈ�� - 1)) ~ (2^(���󰪴� ��Ʈ�� - 1)) - 1
	// UINT~    => 0 ~ (2^(���󰪴� ��Ʈ��))
	// FLOAT    => 4����Ʈ �Ǽ���(�� ��������� ���� ����)
	// TYPELESS => �ؼ� ����� ������ ����(�� ��ü�� ������ �� ����)
	refSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (pConfig->IsVSYNC())
	{
		refSwapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRate.Numerator; // ����
		refSwapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRate.Denominator; // �и�
	}
	else
	{
		refSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0; // ����
		refSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // �и�
	}

	refSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���۹� �뵵
	refSwapChainDesc.OutputWindow = hWnd; // �������� ������ �ڵ�
	refSwapChainDesc.SampleDesc.Count = 1; // �ȼ��� MSAA ����

	// ��ü ȭ���� ���� �����մϴ�.
	// â���� ��ü â���� �ᱹ â����Դϴ�.
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

	// ����� ������ �����ɴϴ�.
	m_spAdpater = std::make_shared<DX11Adapter>();
	TEST_COM(m_spAdpater->StartUp(), hRet);

	// ����� ������ ȣȯ�Ǵ� �ֻ��� ���� ������ �˾Ƴ��ϴ�.
	DXGI_RATIONAL refreshRate;
	m_spAdpater->FindValidRefeshRate(m_pConfig->GetClientWidth(), m_pConfig->GetClientHeight(), refreshRate);

	// ����ü�� ������ �����մϴ�.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	::ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	m_spInside->InitSwapChainDesc(hWnd, refreshRate, swapChainDesc);

	// ������ ������ ����ü��, ����̽�, ����̽� ���ؽ�Ʈ�� �����մϴ�.
	TEST_COM(D3D11CreateDeviceAndSwapChain(
		nullptr, // �⺻ ����� ���
		D3D_DRIVER_TYPE_HARDWARE, // �ϵ���� ���
		nullptr, // �ϵ��� ����ϹǷ� ����
		0, // �÷��� ����
		nullptr, // ��ó������ �˾Ƽ� ����
		0, // ��ó���� ������ 0
		D3D11_SDK_VERSION, // �� ������ ����
		&swapChainDesc, // ������ ����ü�� ����
		m_spSwapChain.GetAddressOf(), // ������ ����ü��
		m_spDevice.GetAddressOf(), // ������ ����̽�
		nullptr, // ������� �����Ƿ� ����
		m_spDeviceCtx.GetAddressOf()), // ������ ����̽� ���ؽ�Ʈ
		hRet);

	// ������ ����۸� �����ɴϴ�. (������ ���ϸ� �̹���, �� �ؽ�ó)
	Microsoft::WRL::ComPtr<ID3D11Texture2D> spBackBufferTex;
	TEST_COM(m_spSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(spBackBufferTex.GetAddressOf())), hRet);

	// ����۸� ����Ÿ�ٺ信 �����մϴ�.
	// �������� ����� �� �� �ְ� �ϴ� �۾��̶�� ���� �˴ϴ�.
	TEST_COM(m_spDevice->CreateRenderTargetView(spBackBufferTex.Get(), nullptr, m_spRenderTargetView.GetAddressOf()), hRet);

	// �������� ����Ÿ�ٺ並 �����մϴ�.
	m_spDeviceCtx->OMSetRenderTargets(1, m_spRenderTargetView.GetAddressOf(), nullptr);
#pragma endregion

	return S_OK;
}

void DX11Context::CleanUp()
{
	// �����ϱ� ������ �׻� â���� �����ؾ� �����ϰ� ������ �� �ֽ��ϴ�.
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
		m_spSwapChain->Present(1, 0); // ���� ������ �����մϴ�.
	}
	else
	{
		m_spSwapChain->Present(0, 0); // �ִ��� ������ �����մϴ�.
	}
}
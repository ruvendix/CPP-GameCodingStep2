#pragma once

class Config;
class Graphics;

class DX11Context
{
public:
	DX11Context(Graphics* pGFX, Config* pConfig);
	~DX11Context() = default;

	HRESULT StartUp(HWND hWnd);
	void CleanUp();

	void BeginRender();
	void EndRender();

	IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }

private:
	Graphics* m_pGFX = nullptr;
	Config* m_pConfig = nullptr;

	IDXGISwapChain* m_pSwapChain = nullptr;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceCtx = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

	DirectX::XMFLOAT4 m_clearColor;
};
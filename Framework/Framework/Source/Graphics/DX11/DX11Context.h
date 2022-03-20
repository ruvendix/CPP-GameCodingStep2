#pragma once

#include "PCH.h"

class Config;
class Graphics;
class DX11Adapter;

class DX11Context
{
	OUTSIDE_CLASS(DX11ContextInside);

public:
	DX11Context(Graphics* pGfx, Config* pConfig);
	~DX11Context() = default;

	HRESULT StartUp(HWND hWnd);
	void CleanUp();

	void BeginRender();
	void EndRender();

	void RefreshSwapChain();

	void SetClearColor(const DirectX::XMFLOAT4 clearColor) { m_clearColor = clearColor; }

	IDXGISwapChain* GetNativeSwapChain() const { return m_spSwapChain.Get(); }
	ID3D11Device* GetNativeDevice() const { return m_spDevice.Get(); }
	ID3D11DeviceContext* GetNativeDeviceContext() const { return m_spDeviceCtx.Get(); }

private:
	Graphics* m_pGfx = nullptr;
	Config* m_pConfig = nullptr;

	std::shared_ptr<DX11Adapter> m_spAdpater;

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_spSwapChain;

	Microsoft::WRL::ComPtr<ID3D11Device> m_spDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_spDeviceCtx;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_spRenderTargetView;

	DirectX::XMFLOAT4 m_clearColor;
};
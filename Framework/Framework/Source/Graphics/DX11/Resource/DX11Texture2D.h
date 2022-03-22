#pragma once

#include "PCH.h"

class Graphics;

class DX11Texture2D
{
public:
	DX11Texture2D() = default;
	~DX11Texture2D() = default;

	HRESULT LoadTexture(const std::wstring& wstrTexturePath, Graphics* pGfx);

	ID3D11ShaderResourceView** GetNativeShaderResourceViewAddress() { return m_spShaderResourceView.GetAddressOf(); }

private:
	std::wstring m_wstrTexturePath;
	DirectX::TexMetadata m_metadata;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_spTex2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_spShaderResourceView;
};
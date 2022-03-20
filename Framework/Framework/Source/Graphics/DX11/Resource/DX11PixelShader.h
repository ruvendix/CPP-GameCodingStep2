#pragma once

#include "PCH.h"

class Shader;
class Graphics;

class DX11PixelShader
{
public:
	DX11PixelShader() = default;
	~DX11PixelShader() = default;

	void CreatePixelShader(Shader* pPixelShader, Graphics* pGfx);

	ID3D11PixelShader* GetNativePixelShader() const { return m_spPixelShader.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_spPixelShader;
};
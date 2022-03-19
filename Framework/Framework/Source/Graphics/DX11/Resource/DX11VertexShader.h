#pragma once

#include "PCH.h"

class Shader;
class Graphics;

class DX11VertexShader
{
public:
	DX11VertexShader() = default;
	~DX11VertexShader() = default;

	void CreateVertexShader(Shader* pShader, Graphics* pGfx);

	ID3D11VertexShader* GetVertexShader() const { return m_spVertexShader.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_spVertexShader;
};
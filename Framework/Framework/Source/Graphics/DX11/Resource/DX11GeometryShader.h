#pragma once

#include "PCH.h"

class Shader;
class Graphics;

class DX11GeometryShader
{
public:
	DX11GeometryShader() = default;
	~DX11GeometryShader() = default;

	void CreateGeometryShader(Shader* pGeometryShader, Graphics* pGfx);

	ID3D11GeometryShader* GetNativeGeometryShader() const { return m_spGeometryShader.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_spGeometryShader;
};
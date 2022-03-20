#pragma once

#include "PCH.h"

enum class ESemanticType
{
	POSITION = 0,
	SCALE,
};

class Shader;
class Graphics;

class DX11InputLayout
{
public:
	static const char* FindSemantic(ESemanticType semanticType);

	DX11InputLayout() = default;
	~DX11InputLayout() = default;

	void CreateInputLayout(ID3DBlob* pVertexShaderBlob, Graphics* pGfx, const D3D11_INPUT_ELEMENT_DESC* pDesc, UINT elemCount);

	ID3D11InputLayout* GetNativeInputLayout() const { return m_spInputLayout.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_spInputLayout;
};
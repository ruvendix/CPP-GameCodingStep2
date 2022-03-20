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
	DX11InputLayout() = default;
	~DX11InputLayout() = default;

	void AddElement(ESemanticType semanticType, DXGI_FORMAT format);
	void CreateInputLayout(Shader* pVertexShader, Graphics* pGfx);

	const char* FindSemantic(ESemanticType semanticType) const;

	ID3D11InputLayout* GetInputLayout() const { return m_spInputLayout.Get(); }

private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_vecDesc;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_spInputLayout;
};
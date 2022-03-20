#pragma once

#include "PCH.h"
#include "Graphics/DX11/Resource/DX11InputLayout.h"

class Shader;
class Graphics;
class DX11InputLayout;

class DX11VertexShader
{
public:
	DX11VertexShader() = default;
	~DX11VertexShader() = default;

	void CreateVertexShader(Shader* pShader, Graphics* pGfx);
	void CreateInputLayout(Graphics* pGfx, const D3D11_INPUT_ELEMENT_DESC* pDesc, UINT elemCount);

	ID3D11VertexShader* GetNativeVertexShader() const { return m_spVertexShader.Get(); }

	DX11InputLayout* GetInputLayout() const { return m_spInputLayout.get(); }
	ID3D11InputLayout* GetNativeInputLayout() const { return m_spInputLayout->GetNativeInputLayout(); }

private:
	ID3DBlob* m_pBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_spVertexShader;
	std::shared_ptr<DX11InputLayout> m_spInputLayout;
};
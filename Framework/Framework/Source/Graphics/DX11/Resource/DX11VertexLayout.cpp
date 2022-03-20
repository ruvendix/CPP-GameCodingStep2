#include "PCH.h"
#include "DX11VertexLayout.h"

#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"

DX11InputLayout DX11VertexPosition::s_inputLayout;
void DX11VertexPosition::GlobalInit(Shader* pVertexShader, Graphics* pGraphics)
{
	s_inputLayout.AddElement(ESemanticType::POSITION, DXGI_FORMAT_R32G32_FLOAT);
	s_inputLayout.CreateInputLayout(pVertexShader, pGraphics);
}

DX11InputLayout DX11VertexPositionScale::s_inputLayout;
void DX11VertexPositionScale::GlobalInit(Shader* pVertexShader, Graphics* pGraphics)
{
	s_inputLayout.AddElement(ESemanticType::POSITION, DXGI_FORMAT_R32G32_FLOAT);
	s_inputLayout.AddElement(ESemanticType::SCALE, DXGI_FORMAT_R32G32_FLOAT);
	s_inputLayout.CreateInputLayout(pVertexShader, pGraphics);
}
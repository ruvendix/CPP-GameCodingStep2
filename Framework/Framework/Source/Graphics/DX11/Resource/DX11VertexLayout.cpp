#include "PCH.h"
#include "DX11VertexLayout.h"

#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/Resource/DX11InputLayout.h"

const D3D11_INPUT_ELEMENT_DESC DX11VertexPosition::InputElemDesc[] =
{
	{ DX11InputLayout::FindSemantic(ESemanticType::POSITION), 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC DX11VertexPositionScale::InputElemDesc[] =
{
	{ DX11InputLayout::FindSemantic(ESemanticType::POSITION), 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ DX11InputLayout::FindSemantic(ESemanticType::SCALE), 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
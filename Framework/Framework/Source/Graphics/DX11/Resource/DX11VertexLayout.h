#pragma once

#include "PCH.h"
#include "Graphics/DX11/Resource/DX11InputLayout.h"

class Shader;
class Graphics;

struct DX11VertexPosition
{
public:
	static const UINT InputElemCount = 1;
	static const D3D11_INPUT_ELEMENT_DESC InputElemDesc[InputElemCount];

	DirectX::XMFLOAT2 pos;
};

struct DX11VertexPositionScale
{
public:
	static const UINT InputElemCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElemDesc[InputElemCount];

	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 scale;
};
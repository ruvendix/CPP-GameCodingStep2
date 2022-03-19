#pragma once

#include "PCH.h"
#include "Graphics/DX11/Resource/DX11InputLayout.h"

class Shader;
class Graphics;

struct DX11VertexPosition
{
public:
	static void GlobalInit(Shader* pVertexShader, Graphics* pGraphics);
	static DX11InputLayout GetInputLayout() { return s_inputLayout; }

	static DX11InputLayout s_inputLayout;
	DirectX::XMFLOAT2 pos;
};
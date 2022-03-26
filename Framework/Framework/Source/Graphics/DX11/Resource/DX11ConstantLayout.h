#pragma once

#include "PCH.h"

struct ConstantBaseMatrix
{
	DirectX::XMFLOAT4X4 matWorld;
	DirectX::XMFLOAT4X4 matView;
	DirectX::XMFLOAT4X4 matProjection;
};
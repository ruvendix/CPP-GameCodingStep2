#pragma once

#include "PCH.h"

class Graphics;

class DX11VertexBuffer
{
public:
	DX11VertexBuffer() = default;
	~DX11VertexBuffer() = default;

	void CreateVertexBuffer(Graphics* pGfx, UINT stride, void* pVertices, UINT vertexCount);

	ID3D11Buffer* GetNativeBuffer() const { return m_spBuffer.Get(); }
	UINT GetStride() const { return m_stride; }

private:
	UINT m_stride = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_spBuffer;
};
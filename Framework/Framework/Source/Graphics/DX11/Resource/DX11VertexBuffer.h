#pragma once

#include "PCH.h"

class Graphics;

class DX11VertexBuffer
{
public:
	DX11VertexBuffer() = default;
	~DX11VertexBuffer() = default;

	void CreateVertexBuffer(Graphics* pGfx, UINT stride, const void* pVertices, UINT vertexCount);

	ID3D11Buffer* GetNativeVertexBuffer() const { return m_spVertexBuffer.Get(); }
	ID3D11Buffer** GetNativeVertexBufferAddressOf() { return m_spVertexBuffer.GetAddressOf(); }
	UINT GetStride() const { return m_stride; }

private:
	UINT m_stride = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_spVertexBuffer;
};
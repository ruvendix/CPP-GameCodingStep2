#include "PCH.h"
#include "DX11VertexBuffer.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

void DX11VertexBuffer::CreateVertexBuffer(Graphics* pGfx, UINT stride, void* pVertices, UINT vertexCount)
{
	m_stride = stride;

	D3D11_BUFFER_DESC desc;
	::ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = stride * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT; // CPU 읽기/쓰기 불가능, GPU 일기/쓰기 가능

	D3D11_SUBRESOURCE_DATA data;
	::ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = pVertices;

	HRESULT hRet = S_OK;
	ID3D11Device* pDevice = pGfx->GetContext()->GetNativeDevice();
	TEST_COM(pDevice->CreateBuffer(&desc, &data, m_spBuffer.GetAddressOf()), hRet);
}
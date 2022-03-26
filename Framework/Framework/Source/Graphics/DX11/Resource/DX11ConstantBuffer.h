#pragma once

#include "PCH.h"
#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

class Graphics;

class DX11ConstantBuffer
{
public:
	DX11ConstantBuffer() = default;
	~DX11ConstantBuffer() = default;

	template <typename TConstantLayout>
	void CreateConstantBuffer(Graphics* pGfx)
	{
		D3D11_BUFFER_DESC constantBufferDesc;
		::ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
		constantBufferDesc.ByteWidth = sizeof(TConstantLayout);
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // CPU에서 읽기/쓰기 가능성 열어둠
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU 쓰기만 허용
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hRet = S_OK;
		TEST_COM(pGfx->GetContext()->GetNativeDevice()->CreateBuffer(&constantBufferDesc, nullptr, m_spConstantBuffer.GetAddressOf()), hRet);
	}

	template <typename TConstantLayout>
	void UpdateSubresource(Graphics* pGfx, TConstantLayout* pConstantLayout)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		::ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		pGfx->GetContext()->GetNativeDeviceContext()->Map(m_spConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		{
			std::memcpy(mappedSubresource.pData, pConstantLayout, sizeof(TConstantLayout)); // CPU -> GPU
		}
		pGfx->GetContext()->GetNativeDeviceContext()->Unmap(m_spConstantBuffer.Get(), 0);
	}

	ID3D11Buffer* GetNativeConstantBuffer() const { return m_spConstantBuffer.Get(); }
	ID3D11Buffer** GetNativeConstantBufferAddressOf() { return m_spConstantBuffer.GetAddressOf(); }
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_spConstantBuffer;
};
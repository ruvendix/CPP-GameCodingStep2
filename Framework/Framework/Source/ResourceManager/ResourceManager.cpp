#include "PCH.h"
#include "ResourceManager.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/DX11/Resource/DX11Texture2D.h"

void ResourceManager::StartUp(Graphics* pGfx)
{
	m_pGfx = pGfx;
}

void ResourceManager::RegisterTexture2D(const std::string& strTexture2D_Tag, const std::wstring& wstrTexture2D_Path)
{
	if (m_mapTexture2D.find(strTexture2D_Tag) != m_mapTexture2D.cend())
	{
		return;
	}

	std::shared_ptr<DX11Texture2D> spTex2D = std::make_shared<DX11Texture2D>();
	spTex2D->LoadTexture(wstrTexture2D_Path, m_pGfx);

	m_mapTexture2D.emplace(strTexture2D_Tag, spTex2D);
}

void ResourceManager::RegisterSamplerState(const std::string& strSamplerStateTag, const D3D11_SAMPLER_DESC& samplerDesc)
{
	if (m_mapSamplerState.find(strSamplerStateTag) != m_mapSamplerState.cend())
	{
		return;
	}

	Microsoft::WRL::ComPtr<ID3D11SamplerState> spSamplerState;
	HRESULT hRet = S_OK;
	TEST_COM(m_pGfx->GetContext()->GetNativeDevice()->CreateSamplerState(&samplerDesc, spSamplerState.GetAddressOf()), hRet);

	m_mapSamplerState.emplace(strSamplerStateTag, spSamplerState);
}

std::shared_ptr<Shaders> ResourceManager::FindShaders(const std::string& strShadersTag)
{
	const auto& iter = m_mapShaders.find(strShadersTag);
	if (iter == m_mapShaders.cend())
	{
		return nullptr;
	}

	return (iter->second);
}

std::shared_ptr<DX11VertexBuffer> ResourceManager::FindVertexBuffer(const std::string& strVertexBufferTag)
{
	const auto& iter = m_mapVertexBuffer.find(strVertexBufferTag);
	if (iter == m_mapVertexBuffer.cend())
	{
		return nullptr;
	}

	return (iter->second);
}

std::shared_ptr<DX11ConstantBuffer> ResourceManager::FindConstantBuffer(const std::string& strConstantBufferTag)
{
	const auto& iter = m_mapConstantBuffer.find(strConstantBufferTag);
	if (iter == m_mapConstantBuffer.cend())
	{
		return nullptr;
	}

	return (iter->second);
}

std::shared_ptr<DX11Texture2D> ResourceManager::FindTexture2D(const std::string& strTexture2D_Tag)
{
	const auto& iter = m_mapTexture2D.find(strTexture2D_Tag);
	if (iter == m_mapTexture2D.cend())
	{
		return nullptr;
	}

	return (iter->second);
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> ResourceManager::FindSamplerState(const std::string& strSamplerStateTag)
{
	const auto& iter = m_mapSamplerState.find(strSamplerStateTag);
	if (iter == m_mapSamplerState.cend())
	{
		return nullptr;
	}

	return (iter->second);
}

void ResourceManager::ApplyGraphicsPipelneAndRender2D(const std::string& strShadersTag, const std::string& strVertexBufferTag,
	const std::string& strConstantBufferTag, const std::string& strTexture2D_Tag, const std::string& strSamplerStateTag)
{
	ID3D11DeviceContext* pDeviceCtx = m_pGfx->GetContext()->GetNativeDeviceContext();

	// 필요한 것들 미리 가져옴
	std::shared_ptr<Shaders> spShaders = FindShaders(strShadersTag);
	std::shared_ptr<DX11ConstantBuffer> spConstantBuffer = FindConstantBuffer(strConstantBufferTag);
	//ID3D11Buffer* pConstantBuffer = spVertexBuffer->GetNativeVertexBuffer();

#pragma region IA
	pDeviceCtx->IASetInputLayout(spShaders->spVertexShader->GetNativeInputLayout());

	std::shared_ptr<DX11VertexBuffer> spVertexBuffer = FindVertexBuffer(strVertexBufferTag);
	UINT offset = 0;
	UINT stride = spVertexBuffer->GetStride();
	pDeviceCtx->IASetVertexBuffers(0, 1, spVertexBuffer->GetNativeVertexBufferAddressOf(), &stride, &offset);
	
	pDeviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
#pragma endregion
	
#pragma region VS
	pDeviceCtx->VSSetShader(spShaders->spVertexShader->GetNativeVertexShader(), nullptr, 0);
	pDeviceCtx->VSSetConstantBuffers(0, 1, spConstantBuffer->GetNativeConstantBufferAddressOf());
#pragma endregion

#pragma region GS
	pDeviceCtx->GSSetShader(spShaders->spGeometryShader->GetNativeGeometryShader(), nullptr, 0);
	pDeviceCtx->GSSetConstantBuffers(0, 1, spConstantBuffer->GetNativeConstantBufferAddressOf());
#pragma endregion

#pragma region PS
	pDeviceCtx->PSSetShader(spShaders->spPixelShader->GetNativePixelShader(), nullptr, 0);

	std::shared_ptr<DX11Texture2D> spTexture2D = FindTexture2D(strTexture2D_Tag);
	if (spTexture2D == nullptr)
	{
		// 기본 텍스처로 출력하거나 그런듯?
		spTexture2D = m_mapTexture2D["Null"];
	}
	pDeviceCtx->PSSetShaderResources(0, 1, spTexture2D->GetNativeShaderResourceViewAddress());

	Microsoft::WRL::ComPtr<ID3D11SamplerState> spSamplerState = FindSamplerState(strSamplerStateTag);
	pDeviceCtx->PSSetSamplers(0, 1, spSamplerState.GetAddressOf());
#pragma endregion

	pDeviceCtx->Draw(1, 0);
}
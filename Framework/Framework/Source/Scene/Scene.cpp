#include "PCH.h"
#include "Scene.h"

#include "ErrorHandler/ErrorHandler.h"

#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"
#include "Graphics/DX11/Resource/DX11VertexShader.h"
#include "Graphics/DX11/Resource/DX11PixelShader.h"
#include "Graphics/DX11/Resource/DX11GeometryShader.h"
#include "Graphics/DX11/Resource/DX11VertexLayout.h"
#include "Graphics/DX11/Resource/DX11VertexBuffer.h"
#include "Graphics/DX11/Resource/DX11Texture2D.h"

Scene::Scene(const std::string& strName, Graphics* pGfx) :
	m_strName(strName)
{
	m_pGfx = pGfx;
}

void Scene::StartUp()
{
#pragma region ���ؽ� ���̴�
	std::shared_ptr<Shader> spVertexShader = std::make_shared<Shader>(L"DefaultVS", EShaderType::VERTEX_SHADER);
	spVertexShader->LoadShader();

	std::vector<DX11VertexPositionScale> vecVertex;
	vecVertex.push_back(DX11VertexPositionScale{ DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.25f) });

	m_spVertexShader = std::make_shared<DX11VertexShader>();
	m_spVertexShader->CreateVertexShader(spVertexShader.get(), m_pGfx);
	m_spVertexShader->CreateInputLayout(m_pGfx, DX11VertexPositionScale::InputElemDesc, DX11VertexPositionScale::InputElemCount);
#pragma endregion

#pragma region �ȼ� ���̴�
	std::shared_ptr<Shader> spPixelShader = std::make_shared<Shader>(L"DefaultPS", EShaderType::PIXEL_SHADER);
	spPixelShader->LoadShader();
	m_spPixelShader = std::make_shared<DX11PixelShader>();
	m_spPixelShader->CreatePixelShader(spPixelShader.get(), m_pGfx);
#pragma endregion

#pragma region ������Ʈ�� ���̴�
	std::shared_ptr<Shader> spGeometryShader = std::make_shared<Shader>(L"DefaultGS", EShaderType::GEOMETRY_SHADER);
	spGeometryShader->LoadShader();
	m_spGeometryShader = std::make_shared<DX11GeometryShader>();
	m_spGeometryShader->CreateGeometryShader(spGeometryShader.get(), m_pGfx);
#pragma endregion

	m_spVertexBuffer = std::make_shared<DX11VertexBuffer>();
	m_spVertexBuffer->CreateVertexBuffer(m_pGfx, sizeof(DX11VertexPositionScale), vecVertex.data(), vecVertex.size());

	D3D11_SAMPLER_DESC samplerDesc;
	::ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pGfx->GetContext()->GetNativeDevice()->CreateSamplerState(&samplerDesc, m_spSamplerState.GetAddressOf());

	m_spTex2D = std::make_shared<DX11Texture2D>();
	m_spTex2D->LoadTexture(L"Textures/KirbyTitle.jpg", m_pGfx);
}

void Scene::CleanUp()
{

}

void Scene::Update()
{

}

void Scene::Render()
{
	ID3D11DeviceContext* pDeviceCtx = m_pGfx->GetContext()->GetNativeDeviceContext();

	pDeviceCtx->IASetInputLayout(m_spVertexShader->GetNativeInputLayout());
	pDeviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT offset = 0;
	UINT stride = m_spVertexBuffer->GetStride();
	ID3D11Buffer* pVertexBuffer = m_spVertexBuffer->GetNativeBuffer();
	pDeviceCtx->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	pDeviceCtx->VSSetShader(m_spVertexShader->GetNativeVertexShader(), nullptr, 0);
	pDeviceCtx->GSSetShader(m_spGeometryShader->GetNativeGeometryShader(), nullptr, 0);
	pDeviceCtx->PSSetShader(m_spPixelShader->GetNativePixelShader(), nullptr, 0);

	pDeviceCtx->PSSetSamplers(0, 1, m_spSamplerState.GetAddressOf());
	pDeviceCtx->PSSetShaderResources(0, 1, m_spTex2D->GetNativeShaderResourceViewAddress());

	D3D11_BLEND_DESC desc;
	::ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	desc.AlphaToCoverageEnable = TRUE;
	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	Microsoft::WRL::ComPtr<ID3D11BlendState> spBS;
	m_pGfx->GetContext()->GetNativeDevice()->CreateBlendState(&desc, spBS.GetAddressOf());

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	pDeviceCtx->OMSetBlendState(spBS.Get(), blendFactor, 0xffffffff);

	pDeviceCtx->Draw(1, 0);
}
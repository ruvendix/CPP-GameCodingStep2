#include "PCH.h"
#include "Scene.h"

#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"
#include "Graphics/DX11/Resource/DX11VertexShader.h"
#include "Graphics/DX11/Resource/DX11PixelShader.h"
#include "Graphics/DX11/Resource/DX11GeometryShader.h"
#include "Graphics/DX11/Resource/DX11VertexLayout.h"
#include "Graphics/DX11/Resource/DX11VertexBuffer.h"

Scene::Scene(const std::string& strName, Graphics* pGfx) :
	m_strName(strName)
{
	m_pGfx = pGfx;
}

void Scene::StartUp()
{
#pragma region πˆ≈ÿΩ∫ ºŒ¿Ã¥ı
	std::shared_ptr<Shader> spVertexShader = std::make_shared<Shader>(L"DefaultVS", EShaderType::VERTEX_SHADER);
	spVertexShader->LoadShader();

	std::vector<DX11VertexPositionScale> vecVertex;
	vecVertex.push_back(DX11VertexPositionScale{ DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });

	m_spVertexShader = std::make_shared<DX11VertexShader>();
	m_spVertexShader->CreateVertexShader(spVertexShader.get(), m_pGfx);
	m_spVertexShader->CreateInputLayout(m_pGfx, DX11VertexPositionScale::InputElemDesc, DX11VertexPositionScale::InputElemCount);
#pragma endregion

#pragma region «»ºø ºŒ¿Ã¥ı
	std::shared_ptr<Shader> spPixelShader = std::make_shared<Shader>(L"DefaultPS", EShaderType::PIXEL_SHADER);
	spPixelShader->LoadShader();
	m_spPixelShader = std::make_shared<DX11PixelShader>();
	m_spPixelShader->CreatePixelShader(spPixelShader.get(), m_pGfx);
#pragma endregion

#pragma region ¡ˆø¿∏ﬁ∆Æ∏Æ ºŒ¿Ã¥ı
	std::shared_ptr<Shader> spGeometryShader = std::make_shared<Shader>(L"DefaultGS", EShaderType::GEOMETRY_SHADER);
	spGeometryShader->LoadShader();
	m_spGeometryShader = std::make_shared<DX11GeometryShader>();
	m_spGeometryShader->CreateGeometryShader(spGeometryShader.get(), m_pGfx);
#pragma endregion

	m_spVertexBuffer = std::make_shared<DX11VertexBuffer>();
	m_spVertexBuffer->CreateVertexBuffer(m_pGfx, sizeof(DX11VertexPositionScale), vecVertex.data(), vecVertex.size());
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

	pDeviceCtx->Draw(1, 0);
}
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
	std::shared_ptr<Shader> spVertexShader = std::make_shared<Shader>(L"DefaultVS", EShaderType::VERTEX_SHADER);
	spVertexShader->LoadShader();
	m_spVertexShader = std::make_shared<DX11VertexShader>();
	m_spVertexShader->CreateVertexShader(spVertexShader.get(), m_pGfx);
	DX11VertexPositionScale::GlobalInit(spVertexShader.get(), m_pGfx);

	std::shared_ptr<Shader> spPixelShader = std::make_shared<Shader>(L"DefaultPS", EShaderType::PIXEL_SHADER);
	spPixelShader->LoadShader();
	m_spPixelShader = std::make_shared<DX11PixelShader>();
	m_spPixelShader->CreatePixelShader(spPixelShader.get(), m_pGfx);

	std::shared_ptr<Shader> spGeometryShader = std::make_shared<Shader>(L"DefaultGS", EShaderType::GEOMETRY_SHADER);
	spGeometryShader->LoadShader();
	m_spGeometryShader = std::make_shared<DX11GeometryShader>();
	m_spGeometryShader->CreateGeometryShader(spGeometryShader.get(), m_pGfx);

	std::vector<DX11VertexPositionScale> vecVertex;
	vecVertex.push_back(DX11VertexPositionScale{DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)});

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
	ID3D11DeviceContext* pDeviceCtx = m_pGfx->GetContext()->GetDeviceContext();

	pDeviceCtx->IASetInputLayout(DX11VertexPositionScale::GetInputLayout().GetInputLayout());
	pDeviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = m_spVertexBuffer->GetStride();
	UINT offset = 0;
	ID3D11Buffer* pVertexBuffer = m_spVertexBuffer->GetBuffer();
	pDeviceCtx->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	pDeviceCtx->VSSetShader(m_spVertexShader->GetVertexShader(), nullptr, 0);
	pDeviceCtx->GSSetShader(m_spGeometryShader->GetGeometryShader(), nullptr, 0);
	pDeviceCtx->PSSetShader(m_spPixelShader->GetPixelShader(), nullptr, 0);

	pDeviceCtx->Draw(1, 0);
}
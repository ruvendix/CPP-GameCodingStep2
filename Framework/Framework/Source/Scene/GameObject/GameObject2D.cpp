#include "PCH.h"
#include "GameObject2D.h"

#include "ResourceManager/ResourceManager.h"

#include "InputDevice/InputManager.h"
#include "InputDevice/InputComponent.h"

#include "TransformComponent.h"

#include "Graphics/DX11/Resource/DX11VertexLayout.h"
#include "Graphics/DX11/Resource/DX11ConstantLayout.h"
#include "Graphics/DX11/Resource/DX11ConstantBuffer.h"

GameObject2D::GameObject2D(const std::string& strName) :
	m_strName(strName)
{

}

GameObject2D::~GameObject2D()
{

}

void GameObject2D::StartUp()
{
	// 트랜스폼 생성
	m_spTransform = std::make_shared<TransformComponent>();

	// 입력 리스너 등록
	InputComponent* pInputComponent = SINGLETON(InputManager)->CreateInputComponent();
	pInputComponent->RegisterListener(BIND_INPUT_LISTENER(VK_LEFT, EInputState::DOWN, GameObject2D::OnLeftKeyDown));
	pInputComponent->RegisterListener(BIND_INPUT_LISTENER(VK_LEFT, EInputState::UP, GameObject2D::OnLeftKeyUp));

	// 셰이더
	SINGLETON(ResourceManager)->RegisterShaders<DX11VertexPositionScale>("GameObject2D_Shaders", L"DefaultVS", L"DefaultGS", L"DefaultPS");

	// 버텍스 버퍼
	std::vector<DX11VertexPositionScale> vecVertex;
	vecVertex.push_back(DX11VertexPositionScale{ DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1600.0f, 1200.0f) });
	SINGLETON(ResourceManager)->RegisterVertexBuffer<DX11VertexPositionScale>("GameObject2D_Vertex", vecVertex);

	// 컨스턴트 버퍼 데이터 초기화
	m_spConstantBaseMatrix = std::make_shared<ConstantBaseMatrix>();
	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&(m_spConstantBaseMatrix->matWorld), matWorld);
	DirectX::XMMATRIX matView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat4x4(&(m_spConstantBaseMatrix->matView), matView);
	DirectX::XMMATRIX matProjection = DirectX::XMMatrixOrthographicLH(1600.0f, 1200.0f, 0.001f, 10.0f);
	DirectX::XMStoreFloat4x4(&(m_spConstantBaseMatrix->matProjection), matProjection);

	// 컨스턴트 버퍼
	SINGLETON(ResourceManager)->RegisterConstantBuffer<ConstantBaseMatrix>("BaseMatrix");

	// 텍스처
	SINGLETON(ResourceManager)->RegisterTexture2D("Null", L"Textures/Null.png");
	SINGLETON(ResourceManager)->RegisterTexture2D("KirbyTitle", L"Textures/KirbyTitle.jpg");
	SINGLETON(ResourceManager)->RegisterTexture2D("KirbyAlpha", L"Textures/KirbyAlpha.png");

	// 샘플러 스테이트
	D3D11_SAMPLER_DESC samplerDesc;
	::ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SINGLETON(ResourceManager)->RegisterSamplerState("DefaultSamplerState", samplerDesc);
}

void GameObject2D::Update(Graphics* pGfx)
{
	DirectX::XMMATRIX matWorld = m_spTransform->BuildWorldMatrix();
	DirectX::XMStoreFloat4x4(&(m_spConstantBaseMatrix->matWorld), DirectX::XMMatrixTranspose(matWorld)); // 셰이더에서 사용하려면 전치 필수!

	DirectX::XMMATRIX matView = DirectX::XMLoadFloat4x4(&(m_spConstantBaseMatrix->matView));
	DirectX::XMStoreFloat4x4(&(m_spConstantBaseMatrix->matView), DirectX::XMMatrixTranspose(matView)); // 셰이더에서 사용하려면 전치 필수!

	DirectX::XMMATRIX matProjection = DirectX::XMLoadFloat4x4(&(m_spConstantBaseMatrix->matProjection));
	DirectX::XMStoreFloat4x4(&(m_spConstantBaseMatrix->matProjection), DirectX::XMMatrixTranspose(matProjection)); // 셰이더에서 사용하려면 전치 필수!

	std::shared_ptr<DX11ConstantBuffer> spConstantBuffer = SINGLETON(ResourceManager)->FindConstantBuffer("BaseMatrix");
	spConstantBuffer->UpdateSubresource<ConstantBaseMatrix>(pGfx, m_spConstantBaseMatrix.get());
}

void GameObject2D::Render()
{
	//D3D11_BLEND_DESC desc;
	//::ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	//desc.AlphaToCoverageEnable = TRUE;
	//desc.RenderTarget[0].BlendEnable = TRUE;
	//desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	//desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	//Microsoft::WRL::ComPtr<ID3D11BlendState> spBS;
	//m_pGfx->GetContext()->GetNativeDevice()->CreateBlendState(&desc, spBS.GetAddressOf());
	//float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//pDeviceCtx->OMSetBlendState(spBS.Get(), blendFactor, 0xffffffff);

	SINGLETON(ResourceManager)->ApplyGraphicsPipelneAndRender2D("GameObject2D_Shaders", "GameObject2D_Vertex", "BaseMatrix", m_strTexture2D, "DefaultSamplerState");
}

void GameObject2D::CleanUp()
{

}

void GameObject2D::OnLeftKeyDown()
{
	DirectX::XMFLOAT2 pos = m_spTransform->GetPosition();
	pos.x -= 100.0f;
	m_spTransform->SetPosition(pos);
}

void GameObject2D::OnLeftKeyUp()
{
	DirectX::XMFLOAT2 pos = m_spTransform->GetPosition();
	pos.x = 0.0f;
	m_spTransform->SetPosition(pos);
}
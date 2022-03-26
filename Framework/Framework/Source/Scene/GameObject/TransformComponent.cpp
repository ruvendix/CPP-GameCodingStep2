#include "PCH.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	m_pos = DirectX::XMFLOAT2{ 0.0f, 0.0f };
	m_scale = DirectX::XMFLOAT2{ 1.0f, 1.0f };
}

TransformComponent::~TransformComponent()
{

}

DirectX::XMMATRIX TransformComponent::BuildWorldMatrix()
{
	DirectX::XMMATRIX matScaling = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, 1.0f);

	float rotationRadian = m_rotationAngle * (3.141592f / 180.0f);
	DirectX::XMMATRIX matRotationZ = DirectX::XMMatrixRotationZ(rotationRadian);

	DirectX::XMMATRIX matTranslation = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, 0.0f);

	// 셰이더에서 사용할 때만 전치를 해야 하므로 여기서는 일단 두는 게 맞음
	return (matScaling * matRotationZ * matTranslation);
}
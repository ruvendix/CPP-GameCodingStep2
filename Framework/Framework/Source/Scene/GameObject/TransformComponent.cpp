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

	// ���̴����� ����� ���� ��ġ�� �ؾ� �ϹǷ� ���⼭�� �ϴ� �δ� �� ����
	return (matScaling * matRotationZ * matTranslation);
}
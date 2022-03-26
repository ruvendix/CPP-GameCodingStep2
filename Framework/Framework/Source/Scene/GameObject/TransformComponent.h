#pragma once

#include "PCH.h"

class TransformComponent
{
public:
	TransformComponent();
	~TransformComponent();

	const DirectX::XMFLOAT2& GetPosition() const { return m_pos; }
	void SetPosition(const DirectX::XMFLOAT2& pos) { m_pos = pos; }
	
	float GetRotationAngle() const { return m_rotationAngle; }
	void SetRotationAngle(float rotationAngle) { m_rotationAngle = rotationAngle; }
	
	const DirectX::XMFLOAT2& GetScale() const { return m_scale; }
	void SetScale(const DirectX::XMFLOAT2& scale) { m_scale = scale; }

	DirectX::XMMATRIX BuildWorldMatrix();

private:
	DirectX::XMFLOAT2 m_pos;
	float m_rotationAngle = 0.0f;
	DirectX::XMFLOAT2 m_scale;
};
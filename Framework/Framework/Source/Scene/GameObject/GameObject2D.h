#pragma once

#include "PCH.h"

class TransformComponent;
class Graphics;

struct ConstantBaseMatrix;

class GameObject2D
{
public:
	GameObject2D(const std::string& strName);
	~GameObject2D();

	void StartUp(const DirectX::XMFLOAT2& scale);
	void Update();
	void Render(Graphics* pGfx);
	void CleanUp();

	void OnLeftKeyDown();
	void OnLeftKeyUp();

	void SetTexture2D(const std::string& strTexture2D) { m_strTexture2D = strTexture2D; }
	std::shared_ptr<TransformComponent> GetTransform() const { return m_spTransform; }

private:
	std::string m_strName;
	std::string m_strTexture2D;

	std::shared_ptr<ConstantBaseMatrix> m_spConstantBaseMatrix; // 이거는 분리해야함
	std::shared_ptr<TransformComponent> m_spTransform;
};
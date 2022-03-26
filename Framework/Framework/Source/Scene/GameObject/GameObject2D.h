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

	void StartUp();
	void Update(Graphics* pGfx);
	void Render();
	void CleanUp();

	void OnLeftKeyDown();
	void OnLeftKeyUp();

	void SetTexture2D(const std::string& strTexture2D) { m_strTexture2D = strTexture2D; }

private:
	std::string m_strName;
	std::string m_strTexture2D;

	std::shared_ptr<ConstantBaseMatrix> m_spConstantBaseMatrix; // 이거는 분리해야함
	std::shared_ptr<TransformComponent> m_spTransform;
};
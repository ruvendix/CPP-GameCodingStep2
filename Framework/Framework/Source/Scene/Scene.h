#pragma once

#include "PCH.h"

class Graphics;
class GameObject2D;

struct ConstantBaseMatrix;

class Scene
{
public:
	Scene(const std::string& strName, Graphics* pGfx);
	~Scene() = default;

	void StartUp();
	void Update();
	void Render();
	void CleanUp();

private:
	Graphics* m_pGfx = nullptr;

	std::string m_strName;
	std::shared_ptr<GameObject2D> spGameObject2D;
};
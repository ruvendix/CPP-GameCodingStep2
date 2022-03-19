#pragma once

#include "PCH.h"

class Graphics;
class Shader;

class Scene
{
public:
	Scene(const std::string& strName, Graphics* pGFX);
	~Scene() = default;

	void StartUp();
	void CleanUp();

	void Update();
	void Render();

private:
	std::string m_strName;

	Graphics* m_pGFX = nullptr;
	Shader* m_pShader = nullptr;
};
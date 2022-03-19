#pragma once

#include "PCH.h"

class Graphics;

class Scene
{
public:
	Scene(const std::string& strName);
	~Scene() = default;

	void StartUp();
	void CleanUp();

	void Update();
	void Render(Graphics* pGFX);

private:
	std::string m_strName;
};
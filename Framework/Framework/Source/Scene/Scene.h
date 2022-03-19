#pragma once

#include "PCH.h"

class Graphics;
class DX11VertexShader;
class DX11PixelShader;
class DX11VertexBuffer;

class Scene
{
public:
	Scene(const std::string& strName, Graphics* pGfx);
	~Scene() = default;

	void StartUp();
	void CleanUp();

	void Update();
	void Render();

private:
	std::string m_strName;

	Graphics* m_pGfx = nullptr;

	std::shared_ptr<DX11VertexShader> m_spVertexShader;
	std::shared_ptr<DX11PixelShader> m_spPixelShader;
	std::shared_ptr<DX11VertexBuffer> m_spVertexBuffer;
};
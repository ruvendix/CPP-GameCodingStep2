#pragma once

#include "PCH.h"

class Graphics;
class DX11VertexShader;
class DX11PixelShader;
class DX11GeometryShader;
class DX11VertexBuffer;
class DX11Texture2D;

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
	std::shared_ptr<DX11GeometryShader> m_spGeometryShader;
	std::shared_ptr<DX11PixelShader> m_spPixelShader;
	std::shared_ptr<DX11VertexBuffer> m_spVertexBuffer;
	std::shared_ptr<DX11Texture2D> m_spTex2D;

	// �ϴ��� �Ǵ��� �׽�Ʈ
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_spSamplerState;
};
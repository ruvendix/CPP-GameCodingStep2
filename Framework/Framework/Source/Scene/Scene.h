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
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_spConstantBuffer;

	DirectX::XMFLOAT4X4 m_matWorld;
	DirectX::XMFLOAT4X4 m_matView;
	DirectX::XMFLOAT4X4 m_matProjection;

	struct MatrixBuffer
	{
		DirectX::XMFLOAT4X4 matWorld;
		DirectX::XMFLOAT4X4 matView;
		DirectX::XMFLOAT4X4 matProjection;
	} m_resultMatrix;
};
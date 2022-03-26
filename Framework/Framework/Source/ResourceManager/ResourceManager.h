#pragma once

#include "PCH.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h" // 임시
#include "Graphics/DX11/DX11Context.h" // 임시
#include "Graphics/DX11/Resource/DX11VertexShader.h"
#include "Graphics/DX11/Resource/DX11PixelShader.h"
#include "Graphics/DX11/Resource/DX11GeometryShader.h"
#include "Graphics/DX11/Resource/DX11VertexBuffer.h"
#include "Graphics/DX11/Resource/DX11ConstantBuffer.h"

class DX11Texture2D;

struct Shaders
{
	std::shared_ptr<DX11VertexShader> spVertexShader;
	std::shared_ptr<DX11GeometryShader> spGeometryShader;
	std::shared_ptr<DX11PixelShader> spPixelShader;
};

class ResourceManager
{
	GENERATE_SINGLETON(ResourceManager);

public:
	void StartUp(Graphics* pGfx);

	void RegisterTexture2D(const std::string& strTexture2D_Tag, const std::wstring& wstrTexture2D_Path);
	void RegisterSamplerState(const std::string& strSamplerStateTag, const D3D11_SAMPLER_DESC& samplerDesc);

	std::shared_ptr<Shaders> FindShaders(const std::string& strShadersTag);
	std::shared_ptr<DX11VertexBuffer> FindVertexBuffer(const std::string& strVertexBufferTag);
	std::shared_ptr<DX11ConstantBuffer> FindConstantBuffer(const std::string& strConstantBufferTag);
	std::shared_ptr<DX11Texture2D> FindTexture2D(const std::string& strTexture2D_Tag);
	Microsoft::WRL::ComPtr<ID3D11SamplerState> FindSamplerState(const std::string& strSamplerStateTag);

	void ApplyGraphicsPipelneAndRender2D(const std::string& strShadersTag, const std::string& strVertexBufferTag,
		const std::string& strConstantBufferTag, const std::string& strTexture2D_Tag, const std::string& strSamplerStateTag);

	template <typename TVertexLayout>
	void RegisterShaders(const std::string& strShadersTag, const std::wstring& wstrVertexShader,
		const std::wstring& wstrGeometryShader, const std::wstring& wstrPixelShader)
	{
		if (m_mapShaders.find(strShadersTag) != m_mapShaders.cend())
		{
			return;
		}

		std::shared_ptr<Shaders> spShaders = std::make_shared<Shaders>();

#pragma region 버텍스 셰이더
		std::shared_ptr<Shader> spVertexShaderBase = std::make_shared<Shader>();
		spVertexShaderBase->LoadShader(wstrVertexShader, EShaderType::VERTEX_SHADER);
		std::shared_ptr<DX11VertexShader> spVertexShader = std::make_shared<DX11VertexShader>();
		spVertexShader->CreateVertexShader(spVertexShaderBase.get(), m_pGfx);
		spVertexShader->CreateInputLayout(m_pGfx, TVertexLayout::InputElemDesc, TVertexLayout::InputElemCount);
		spShaders->spVertexShader = spVertexShader;
#pragma endregion

#pragma region 지오메트리 셰이더
		std::shared_ptr<Shader> spGeometryShaderBase = std::make_shared<Shader>();
		spGeometryShaderBase->LoadShader(wstrGeometryShader, EShaderType::GEOMETRY_SHADER);
		std::shared_ptr<DX11GeometryShader> spGeometryShader = std::make_shared<DX11GeometryShader>();
		spGeometryShader->CreateGeometryShader(spGeometryShaderBase.get(), m_pGfx);
		spShaders->spGeometryShader = spGeometryShader;
#pragma endregion

#pragma region 픽셀 셰이더
		std::shared_ptr<Shader> spPixelShaderBase = std::make_shared<Shader>();
		spPixelShaderBase->LoadShader(wstrPixelShader, EShaderType::PIXEL_SHADER);
		std::shared_ptr<DX11PixelShader> spPixelShader = std::make_shared<DX11PixelShader>();
		spPixelShader->CreatePixelShader(spPixelShaderBase.get(), m_pGfx);
		spShaders->spPixelShader = spPixelShader;
#pragma endregion

		m_mapShaders.emplace(strShadersTag, spShaders);
	}

	template <typename TVertexLayout>
	void RegisterVertexBuffer(const std::string& strVertexBufferTag,  const std::vector<TVertexLayout>& vecVertex)
	{
		if (m_mapVertexBuffer.find(strVertexBufferTag) != m_mapVertexBuffer.cend())
		{
			return;
		}

		std::shared_ptr<DX11VertexBuffer> spVertexBuffer = std::make_shared<DX11VertexBuffer>();
		spVertexBuffer->CreateVertexBuffer(m_pGfx, sizeof(TVertexLayout), vecVertex.data(), vecVertex.size());

		m_mapVertexBuffer.emplace(strVertexBufferTag, spVertexBuffer);
	}

	template <typename TConstantLayout> // 가독성을 위한 템플릿?
	void RegisterConstantBuffer(const std::string& strConstantBufferTag)
	{
		if (m_mapConstantBuffer.find(strConstantBufferTag) != m_mapConstantBuffer.cend())
		{
			return;
		}

		std::shared_ptr<DX11ConstantBuffer> spConstantBuffer = std::make_shared<DX11ConstantBuffer>();
		spConstantBuffer->CreateConstantBuffer<TConstantLayout>(m_pGfx);

		m_mapConstantBuffer.emplace(strConstantBufferTag, spConstantBuffer);
	}

private:
	Graphics* m_pGfx = nullptr;

	std::unordered_map<std::string, std::shared_ptr<DX11VertexBuffer>> m_mapVertexBuffer;
	std::unordered_map<std::string, std::shared_ptr<Shaders>> m_mapShaders;
	std::unordered_map<std::string, std::shared_ptr<DX11ConstantBuffer>> m_mapConstantBuffer;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_mapSamplerState;
	std::unordered_map<std::string, std::shared_ptr<DX11Texture2D>> m_mapTexture2D;
};
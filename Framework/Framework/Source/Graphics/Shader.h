#pragma once

#include "PCH.h"

enum class EShaderType
{
	UNKNOWN = 0,
	VERTEX_SHADER,
	PIXEL_SHADER,
};

class Graphics;

class Shader
{
public:
	Shader(const std::wstring& wstrFileName, EShaderType shaderType);
	~Shader() = default;

	void LoadShader();

	const char* FindShaderTarget(EShaderType shaderType) const;

	ID3DBlob* GetBlob() const { return m_spBlob.Get(); }

private:
	std::wstring m_wstrFileName;
	EShaderType m_shaderType = EShaderType::UNKNOWN;

	Microsoft::WRL::ComPtr<ID3DBlob> m_spBlob;
};
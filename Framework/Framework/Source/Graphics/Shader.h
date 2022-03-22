#pragma once

#include "PCH.h"

enum class EShaderType
{
	UNKNOWN = 0,
	VERTEX_SHADER,
	PIXEL_SHADER,
	GEOMETRY_SHADER,
};

class Graphics;

class Shader
{
public:
	static const char* FindShaderTarget(EShaderType shaderType);

	Shader() = default;
	~Shader() = default;

	void LoadShader(const std::wstring& wstrFileName, EShaderType shaderType);

	ID3DBlob* GetBlob() const { return m_spBlob.Get(); }

private:
	std::wstring m_wstrFileName;
	EShaderType m_shaderType = EShaderType::UNKNOWN;

	Microsoft::WRL::ComPtr<ID3DBlob> m_spBlob;
};
#include "PCH.h"
#include "Shader.h"

#include "ErrorHandler/ErrorHandler.h"

Shader::Shader(const std::wstring& wstrFileName, EShaderType shaderType) :
	m_wstrFileName(wstrFileName)
{
	m_shaderType = shaderType;
}

void Shader::LoadShader()
{
	std::wstring wstrShaderCompiledObjectPath = L"../Build/" + m_wstrFileName + L".cso";

	if (FAILED(D3DReadFileToBlob(wstrShaderCompiledObjectPath.c_str(), m_spBlob.GetAddressOf())))
	{
		::OutputDebugString("cso 파일 읽기 실패!");

		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstrConverter;
		std::string strEntryPoint = wstrConverter.to_bytes(m_wstrFileName);

		Microsoft::WRL::ComPtr<ID3DBlob> spErrorMsg;
		
		// cso 파일이 없다면 직접 컴파일
		std::wstring wstrShaderPath = L"Shader/" + m_wstrFileName + L".hlsl";
		if (FAILED(D3DCompileFromFile(
			wstrShaderPath.c_str(), // hlsl 파일 경로
			nullptr, // 매크로 사용하지 않음
			nullptr, // 포함할 게 없음
			strEntryPoint.c_str(), // 진입점 이름
			FindShaderTarget(m_shaderType), // 셰이더 버전
			D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS, // 컴파일 옵션
			0, // .fx만 사용
			m_spBlob.GetAddressOf(), // Binary Large OBject
			spErrorMsg.GetAddressOf()))) // 오류 발생시 내용을 알릴 곳
		{
			if (spErrorMsg != nullptr)
			{
				ErrorHandler::GetInstance().ShowCustomError(reinterpret_cast<const char*>(spErrorMsg->GetBufferPointer()));
			}
			else
			{
				ErrorHandler::GetInstance().ShowCustomError("셰이더 파일이 존재하지 않습니다!");
			}
		}
	}
}

const char* Shader::FindShaderTarget(EShaderType shaderType) const
{
	switch (shaderType)
	{
	case EShaderType::VERTEX_SHADER:
	{
		return "vs_5_0";
	}

	case EShaderType::PIXEL_SHADER:
	{
		return "ps_5_0";
	}

	case EShaderType::GEOMETRY_SHADER:
	{
		return "gs_5_0";
	}
	}

	return nullptr;
}
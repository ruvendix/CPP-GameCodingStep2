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
		::OutputDebugString("cso ���� �б� ����!");

		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstrConverter;
		std::string strEntryPoint = wstrConverter.to_bytes(m_wstrFileName);

		Microsoft::WRL::ComPtr<ID3DBlob> spErrorMsg;
		
		// cso ������ ���ٸ� ���� ������
		std::wstring wstrShaderPath = L"Shader/" + m_wstrFileName + L".hlsl";
		if (FAILED(D3DCompileFromFile(
			wstrShaderPath.c_str(), // hlsl ���� ���
			nullptr, // ��ũ�� ������� ����
			nullptr, // ������ �� ����
			strEntryPoint.c_str(), // ������ �̸�
			FindShaderTarget(m_shaderType), // ���̴� ����
			D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS, // ������ �ɼ�
			0, // .fx�� ���
			m_spBlob.GetAddressOf(), // Binary Large OBject
			spErrorMsg.GetAddressOf()))) // ���� �߻��� ������ �˸� ��
		{
			if (spErrorMsg != nullptr)
			{
				ErrorHandler::GetInstance().ShowCustomError(reinterpret_cast<const char*>(spErrorMsg->GetBufferPointer()));
			}
			else
			{
				ErrorHandler::GetInstance().ShowCustomError("���̴� ������ �������� �ʽ��ϴ�!");
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
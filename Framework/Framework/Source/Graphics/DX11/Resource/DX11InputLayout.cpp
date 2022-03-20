#include "PCH.h"
#include "DX11InputLayout.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

const char* DX11InputLayout::FindSemantic(ESemanticType semanticType)
{
	switch (semanticType)
	{
	case ESemanticType::POSITION:
	{
		return "POSITION";
	}

	case ESemanticType::SCALE:
	{
		return "SCALE";
	}
	}

	return nullptr;
}

void DX11InputLayout::CreateInputLayout(ID3DBlob* pVertexShaderBlob, Graphics* pGfx, const D3D11_INPUT_ELEMENT_DESC* pDesc, UINT elemCount)
{
	HRESULT hRet = S_OK;
	ID3D11Device* pDevice = pGfx->GetContext()->GetNativeDevice();

	TEST_COM(pDevice->CreateInputLayout(
		pDesc, // �Է��� ������
		elemCount, // �Է��� ���� ����
		pVertexShaderBlob->GetBufferPointer(), // �����ϵ� ���̴� ���� ���� ��ġ
		pVertexShaderBlob->GetBufferSize(), // �����ϵ� ���̴� ���� ũ��
		m_spInputLayout.GetAddressOf()), // ������ ��ǲ ���̾ƿ�
		hRet);
}
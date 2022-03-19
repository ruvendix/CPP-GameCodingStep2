#include "PCH.h"
#include "DX11InputLayout.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

void DX11InputLayout::AddElement(ESemanticType semanticType, DXGI_FORMAT format)
{
	D3D11_INPUT_ELEMENT_DESC desc;
	::ZeroMemory(&desc, sizeof(D3D11_INPUT_ELEMENT_DESC));

	if (m_vecDesc.empty() == false)
	{
		desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	}

	desc.SemanticName = FindSemantic(semanticType);
	desc.Format = format;

	m_vecDesc.push_back(desc);
}

void DX11InputLayout::CreateInputLayout(Shader* pVertexShader, Graphics* pGfx)
{
	HRESULT hRet = S_OK;
	ID3D11Device* pDevice = pGfx->GetContext()->GetDevice();
	ID3DBlob* pBlob = pVertexShader->GetBlob();

	TEST_COM(pDevice->CreateInputLayout(
		m_vecDesc.data(), // �Է��� ������
		m_vecDesc.size(), // �Է��� ���� ����
		pBlob->GetBufferPointer(), // �����ϵ� ���̴� ���� ���� ��ġ
		pBlob->GetBufferSize(), // �����ϵ� ���̴� ���� ũ��
		m_spInputLayout.GetAddressOf()), // ������ ��ǲ ���̾ƿ�
		hRet);
}

const char* DX11InputLayout::FindSemantic(ESemanticType semanticType) const
{
	switch (semanticType)
	{
	case ESemanticType::POSITION:
	{
		return "POSITION";
	}

	case ESemanticType::COLOR:
	{
		return "COLOR";
	}
	}

	return nullptr;
}

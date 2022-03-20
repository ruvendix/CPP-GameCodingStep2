#include "PCH.h"
#include "DX11VertexShader.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

void DX11VertexShader::CreateVertexShader(Shader* pShader, Graphics* pGfx)
{
	HRESULT hRet = S_OK;
	ID3D11Device* pDevice = pGfx->GetContext()->GetNativeDevice();
	m_pBlob = pShader->GetBlob();

	TEST_COM(pDevice->CreateVertexShader(
		m_pBlob->GetBufferPointer(), // �����ϵ� ���̴� ���� ���� ��ġ
		m_pBlob->GetBufferSize(), // �����ϵ� ���̴� ���� ũ��
		nullptr, // ������ ������� ����
		m_spVertexShader.GetAddressOf()), // ������ ���ؽ� ���̴�
		hRet);
}

void DX11VertexShader::CreateInputLayout(Graphics* pGfx, const D3D11_INPUT_ELEMENT_DESC* pDesc, UINT elemCount)
{
	m_spInputLayout = std::make_shared<DX11InputLayout>();
	m_spInputLayout->CreateInputLayout(m_pBlob, pGfx, pDesc, elemCount);
}

#include "PCH.h"
#include "DX11VertexShader.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

void DX11VertexShader::CreateVertexShader(Shader* pShader, Graphics* pGfx)
{
	HRESULT hRet = S_OK;
	ID3D11Device* pDevice = pGfx->GetContext()->GetDevice();
	ID3DBlob* pBlob = pShader->GetBlob();

	TEST_COM(pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(), // �����ϵ� ���̴� ���� ���� ��ġ
		pBlob->GetBufferSize(), // �����ϵ� ���̴� ���� ũ��
		nullptr, // ������ ������� ����
		m_spVertexShader.GetAddressOf()), // ������ ���ؽ� ���̴�
		hRet);
}

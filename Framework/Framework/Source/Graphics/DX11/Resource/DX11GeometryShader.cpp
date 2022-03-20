#include "PCH.h"
#include "DX11GeometryShader.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

void DX11GeometryShader::CreateGeometryShader(Shader* pGeometryShader, Graphics* pGfx)
{
	HRESULT hRet = S_OK;
	ID3D11Device* pDevice = pGfx->GetContext()->GetDevice();
	ID3DBlob* pBlob = pGeometryShader->GetBlob();

	TEST_COM(pDevice->CreateGeometryShader(
		pBlob->GetBufferPointer(), // �����ϵ� ���̴� ���� ���� ��ġ
		pBlob->GetBufferSize(), // �����ϵ� ���̴� ���� ũ��
		nullptr, // ������ ������� ����
		m_spGeometryShader.GetAddressOf()), // ������ ���ؽ� ���̴�
		hRet);
}

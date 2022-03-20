#include "PCH.h"
#include "DX11PixelShader.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

void DX11PixelShader::CreatePixelShader(Shader* pPixelShader, Graphics* pGfx)
{
	HRESULT hRet = S_OK;
	ID3D11Device* pDevice = pGfx->GetContext()->GetNativeDevice();
	ID3DBlob* pBlob = pPixelShader->GetBlob();

	TEST_COM(pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(), // �����ϵ� ���̴� ���� ���� ��ġ
		pBlob->GetBufferSize(), // �����ϵ� ���̴� ���� ũ��
		nullptr, // ������ ������� ����
		m_spPixelShader.GetAddressOf()), // ������ ���ؽ� ���̴�
		hRet);
}

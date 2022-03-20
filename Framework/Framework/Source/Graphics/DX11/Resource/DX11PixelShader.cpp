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
		pBlob->GetBufferPointer(), // 컴파일된 셰이더 파일 시작 위치
		pBlob->GetBufferSize(), // 컴파일된 셰이더 파일 크기
		nullptr, // 지금은 사용하지 않음
		m_spPixelShader.GetAddressOf()), // 생성할 버텍스 셰이더
		hRet);
}

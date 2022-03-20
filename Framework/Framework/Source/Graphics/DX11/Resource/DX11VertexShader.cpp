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
		m_pBlob->GetBufferPointer(), // 컴파일된 셰이더 파일 시작 위치
		m_pBlob->GetBufferSize(), // 컴파일된 셰이더 파일 크기
		nullptr, // 지금은 사용하지 않음
		m_spVertexShader.GetAddressOf()), // 생성할 버텍스 셰이더
		hRet);
}

void DX11VertexShader::CreateInputLayout(Graphics* pGfx, const D3D11_INPUT_ELEMENT_DESC* pDesc, UINT elemCount)
{
	m_spInputLayout = std::make_shared<DX11InputLayout>();
	m_spInputLayout->CreateInputLayout(m_pBlob, pGfx, pDesc, elemCount);
}

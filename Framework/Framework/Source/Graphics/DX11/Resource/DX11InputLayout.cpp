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
		pDesc, // 입력한 정보들
		elemCount, // 입력한 정보 개수
		pVertexShaderBlob->GetBufferPointer(), // 컴파일된 셰이더 파일 시작 위치
		pVertexShaderBlob->GetBufferSize(), // 컴파일된 셰이더 파일 크기
		m_spInputLayout.GetAddressOf()), // 생성할 인풋 레이아웃
		hRet);
}
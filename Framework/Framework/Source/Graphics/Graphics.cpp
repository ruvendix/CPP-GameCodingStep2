#include "PCH.h"
#include "Graphics.h"

#include "DX11/DX11Context.h"

HRESULT Graphics::StartUp(HWND hWnd, Config* pConfig)
{
	m_pDX11Ctx = new DX11Context(this, pConfig);
	return (m_pDX11Ctx->StartUp(hWnd));
}

void Graphics::CleanUp()
{
	m_pDX11Ctx->CleanUp();
	SAFE_DELETE(m_pDX11Ctx);
}

void Graphics::Render()
{
	m_pDX11Ctx->BeginRender();
	{
		// TODO 렌더링 코드 작성
	}
	m_pDX11Ctx->EndRender();
}
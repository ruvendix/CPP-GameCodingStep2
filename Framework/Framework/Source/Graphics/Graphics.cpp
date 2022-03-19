#include "PCH.h"
#include "Graphics.h"

#include "DX11/DX11Context.h"

HRESULT Graphics::StartUp(HWND hWnd, Config* pConfig)
{
	m_spDX11Ctx = std::make_shared<DX11Context>(this, pConfig);
	return (m_spDX11Ctx->StartUp(hWnd));
}

void Graphics::CleanUp()
{
	m_spDX11Ctx->CleanUp();
}

void Graphics::Render()
{
	m_spDX11Ctx->BeginRender();
	{
		// TODO 렌더링 코드 작성
	}
	m_spDX11Ctx->EndRender();
}
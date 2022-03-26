#include "PCH.h"
#include "Graphics.h"

#include "DX11/DX11Context.h"

HRESULT Graphics::StartUp(HWND hWnd)
{
	m_spDX11Ctx = std::make_shared<DX11Context>(this);
	return (m_spDX11Ctx->StartUp(hWnd));
}

void Graphics::CleanUp()
{
	m_spDX11Ctx->CleanUp();
}

void Graphics::BeginRender()
{
	m_spDX11Ctx->BeginRender();
}

void Graphics::EndRender()
{
	m_spDX11Ctx->EndRender();
}
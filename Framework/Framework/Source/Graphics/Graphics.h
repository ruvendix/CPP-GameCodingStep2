#pragma once

#include "PCH.h"

class Config;
class DX11Context;

class Graphics
{
public:
	Graphics() = default;
	~Graphics() = default;

	HRESULT StartUp(HWND hWnd, Config* pConfig);
	void CleanUp();

	void Render();

	DX11Context* GetContext() { return m_spDX11Ctx.get(); }

private:
	std::shared_ptr<DX11Context> m_spDX11Ctx;
};
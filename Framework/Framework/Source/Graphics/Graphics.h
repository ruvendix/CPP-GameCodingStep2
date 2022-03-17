#pragma once

#include "PCH.h"

class Config;
class DX11Context;

class Graphics
{
	GENERATE_SINGLETON(Graphics);

public:
	Graphics() = default;
	~Graphics() = default;

	HRESULT StartUp(HWND hWnd, Config* pConfig);
	void CleanUp();

	void Render();

	const std::string& GetGraphicCardName() const { return m_strGraphicCardName; }
	void SetGraphicCardName(const std::string& strGraphicCardName) { m_strGraphicCardName = strGraphicCardName; }

	SIZE_T GetVRAM_Size() const { return m_VRAM_size; }
	void SetVRAM_Size(SIZE_T VRAM_Size) { m_VRAM_size = VRAM_Size; }

private:
	DX11Context* m_pDX11Ctx = nullptr;

	std::string m_strGraphicCardName;
	SIZE_T m_VRAM_size = 0;
};
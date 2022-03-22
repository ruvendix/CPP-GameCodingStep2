#include "PCH.h"
#include "DX11Texture2D.h"

#include "ErrorHandler/ErrorHandler.h"
#include "Graphics/Graphics.h"
#include "Graphics/DX11/DX11Context.h"

HRESULT DX11Texture2D::LoadTexture(const std::wstring& wstrTexturePath, Graphics* pGfx)
{
	HRESULT hRet = S_OK;

	DirectX::ScratchImage image;
	TEST_COM(DirectX::LoadFromWICFile(
		wstrTexturePath.c_str(),
		DirectX::WIC_FLAGS_NONE,
		&m_metadata,
		image),
		hRet); // ¾ËÆÄºí·»µå ¸Ô¿©¾ß ¾ËÆÄ°¡ »ç¶óÁü

	TEST_COM(DirectX::CreateShaderResourceView(
		pGfx->GetContext()->GetNativeDevice(),
		image.GetImages(), image.GetImageCount(),
		m_metadata,
		m_spShaderResourceView.GetAddressOf()),
		hRet);

	return hRet;
}
#include "Texture.h"
#include "WIC.h"
#include "GraphicsThrowMacros.h"

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& gfx, const std::wstring filename)
{
	INFOMAN(gfx);
	GFX_THROW_INFO(CreateWICTextureFromFile(
		GetDevice(gfx),		//device
		GetContext(gfx),	//context
		filename.c_str(),	//filename
		&pTexture,			//texture
		&pTextureView		//tex view
	));
}

void Texture::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}
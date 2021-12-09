#pragma once
#include "d3dx11effect.h"
#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

struct Font {
	UINT TexWidth;
	UINT TexHeight;

	ID3D11Texture2D* pTexture;
	ID3D11ShaderResourceView* pTextureView;

	std::unique_ptr<Gdiplus::Font> pFont;
	std::unique_ptr<Gdiplus::Bitmap> pCharBitmap;
	std::unique_ptr<Gdiplus::Graphics> pCharGraphics;
	std::unique_ptr<Gdiplus::SolidBrush> pBlackBrush;

	Font(const wchar_t* FontName);
	~Font();
};
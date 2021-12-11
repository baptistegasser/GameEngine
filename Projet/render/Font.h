#pragma once
#include "d3dx11effect.h"
#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

struct Font {
	UINT TextWidth;
	UINT TextHeight;

	ID3D11Texture2D* Texture;
	ID3D11ShaderResourceView* TextureView;

	std::unique_ptr<Gdiplus::Font> FontType;
	std::unique_ptr<Gdiplus::Bitmap> CharBitmap;
	std::unique_ptr<Gdiplus::Graphics> CharGraphics;
	std::unique_ptr<Gdiplus::SolidBrush> BlackBrush;

	Font(const wchar_t* FontName);
	~Font();
};
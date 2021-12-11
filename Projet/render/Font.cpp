#include "stdafx.h"
#include "Font.h"
#include "MoteurWindows.h"
#include "resources/resource.h"
#include "util/util.h"

using namespace DirectX;

Font::Font(const wchar_t* FontName)
	: TextWidth{ 500 }
	, TextHeight{ 100 }
	, Texture{ nullptr }
	, TextureView{ nullptr }
	, CharBitmap{ nullptr }
	, CharGraphics{ nullptr }
	, BlackBrush{ nullptr }
{
	auto& pD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();

	const Gdiplus::FontFamily oFamily(FontName, nullptr);
	FontType = std::make_unique<Gdiplus::Font>(&oFamily, 16.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

	CharBitmap = std::make_unique<Gdiplus::Bitmap>((int) TextWidth, (int) TextHeight, PixelFormat32bppARGB );
	CharGraphics = std::make_unique<Gdiplus::Graphics> (CharBitmap.get() );

	CharGraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
	CharGraphics->SetCompositingQuality(Gdiplus::CompositingQualityHighSpeed);
	CharGraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
	CharGraphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighSpeed);
	CharGraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);
	CharGraphics->SetPageUnit(Gdiplus::UnitPixel);
	Gdiplus::TextRenderingHint hint = Gdiplus::TextRenderingHintAntiAlias;  
	CharGraphics->SetTextRenderingHint(hint);

	BlackBrush = std::make_unique<Gdiplus::SolidBrush> (Gdiplus::Color(255, 0, 0, 0) );

	CharGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));

	// Accéder aux bits du bitmap
	Gdiplus::BitmapData bmData;
	const auto& Rect = Gdiplus::Rect(0, 0, TextWidth, TextHeight);
	CharBitmap->LockBits(&Rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = TextWidth;
	texDesc.Height = TextHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = bmData.Scan0;
	data.SysMemPitch = TextWidth * 4;
	data.SysMemSlicePitch = 0;

	DX_TRY(pD3DDevice.GetD3DDevice()->CreateTexture2D(&texDesc, &data, &Texture));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	DX_TRY(pD3DDevice.GetD3DDevice()->CreateShaderResourceView(Texture, &srvDesc, &TextureView));

	CharBitmap->UnlockBits(&bmData);
}

Font::~Font() {
	PM3D::DXRelacher(TextureView);
	PM3D::DXRelacher(Texture);
}
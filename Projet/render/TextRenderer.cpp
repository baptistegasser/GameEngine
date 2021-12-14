#include "stdafx.h"
#include "TextRenderer.h"
#include "MoteurWindows.h"

using namespace DirectX;

ULONG_PTR TextRenderer::GDIToken{};

TextRenderer::TextRenderer(Pitbull::Actor* Parent, Font* Font, ShaderSprite* Shader, int CanvasHeight, int CanvasWidth)
	: SpriteRenderer{ Parent, Shader, false }
	, FontText{Font}
	, CanvasHeight{CanvasHeight}
	, CanvasWidth{CanvasWidth}
{
	static bool once = []() {
		Gdiplus::GdiplusStartupInput  startupInput(0, TRUE, TRUE);
		Gdiplus::GdiplusStartupOutput startupOutput;

		GdiplusStartup(&GDIToken, &startupInput, &startupOutput);

		return true;
	} ();

	Gdiplus::GdiplusShutdown(GDIToken);

	UpdateCanvas(CanvasHeight, CanvasWidth);
}

TextRenderer::~TextRenderer()
{
	DX_RELEASE(Texture2D);
}

void TextRenderer::Write(const std::wstring& String)
{
	Value = String;

	CharGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));
	CharGraphics->DrawString(Value.c_str(), static_cast<int>(Value.size()), FontText->FontType.get(), Gdiplus::PointF(0.0f, 0.0f), FontText->Brush.get());

	Gdiplus::BitmapData bmData;
	const auto& Rect = Gdiplus::Rect(0, 0, CanvasWidth, CanvasHeight);
	CharBitmap->LockBits(&Rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);

	auto& D3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();

	D3DDevice.GetImmediateContext()->UpdateSubresource(Texture2D, 0, 0, bmData.Scan0, CanvasWidth * 4, 0);

	CharBitmap->UnlockBits(&bmData);
}

void TextRenderer::UpdateCanvas(int NewHeight, int NewWidth)
{
	auto& D3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();

	CanvasHeight = NewHeight;
	CanvasWidth = NewWidth;
	CharBitmap = std::make_unique<Gdiplus::Bitmap>(CanvasWidth, CanvasHeight, PixelFormat32bppARGB);

	CharGraphics = std::make_unique<Gdiplus::Graphics>(CharBitmap.get());
	CharGraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
	CharGraphics->SetCompositingQuality(Gdiplus::CompositingQualityHighSpeed);
	CharGraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
	CharGraphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighSpeed);
	CharGraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);
	CharGraphics->SetPageUnit(Gdiplus::UnitPixel);
	CharGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	Gdiplus::BitmapData bmData{};
	const auto& Rect = Gdiplus::Rect(0, 0, CanvasWidth, CanvasHeight);
	CharBitmap->LockBits(&Rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = CanvasWidth;
	texDesc.Height = CanvasHeight;
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
	data.SysMemPitch = CanvasWidth * 4;
	data.SysMemSlicePitch = 0;

	DX_TRY(D3DDevice.GetD3DDevice()->CreateTexture2D(&texDesc, &data, &Texture2D));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	DX_TRY(D3DDevice.GetD3DDevice()->CreateShaderResourceView(Texture2D, &srvDesc, &TextureView));

	CharBitmap->UnlockBits(&bmData);

	Dimension.x = static_cast<float>(CanvasWidth) * 2.0f / D3DDevice.GetLargeur();
	Dimension.y = static_cast<float>(CanvasHeight) * 2.0f / D3DDevice.GetHauteur();
}

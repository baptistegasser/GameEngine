#include "stdafx.h"
#include "TextRenderer.h"
#include "MoteurWindows.h"

using namespace DirectX;

TextRenderer::TextRenderer(Pitbull::Actor* Parent, Font* Sprite, ShaderSprite* Shader)
	: SpriteRenderer{ Parent, Sprite->TextureView, Shader, false }
	, FontText{Sprite}
{
	//auto& pD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();

	//// Accéder aux bits du bitmap
	//Gdiplus::BitmapData bmData;
	//const auto& Rect = Gdiplus::Rect(0, 0, Sprite->TexWidth, Sprite->TexHeight);
	//Sprite->pCharBitmap->LockBits(&Rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);

	//// Création d'une texture de même dimension sur la carte graphique
	//D3D11_TEXTURE2D_DESC texDesc;
	//texDesc.Width = Sprite->TexWidth;
	//texDesc.Height = Sprite->TexHeight;
	//texDesc.MipLevels = 1;
	//texDesc.ArraySize = 1;
	//texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//texDesc.SampleDesc.Count = 1;
	//texDesc.SampleDesc.Quality = 0;
	//texDesc.Usage = D3D11_USAGE_DEFAULT;
	//texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//texDesc.CPUAccessFlags = 0;
	//texDesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA data;
	//data.pSysMem = bmData.Scan0;
	//data.SysMemPitch = Sprite->TexWidth * 4;
	//data.SysMemSlicePitch = 0;

	//// Création de la texture à partir des données du bitmap
	//DX_TRY(pD3DDevice.GetD3DDevice()->CreateTexture2D(&texDesc, &data, &pTexture));

	//// Création d'un «resourve view» pour accéder facilement à la texture
	////     (comme pour les sprites)
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MipLevels = 1;
	//srvDesc.Texture2D.MostDetailedMip = 0;

	//DX_TRY(pD3DDevice.GetD3DDevice()->CreateShaderResourceView(pTexture, &srvDesc, &TextureView));

	//Sprite->pCharBitmap->UnlockBits(&bmData);

}

void TextRenderer::Write(const std::wstring& String)
{
	// Effacer
	FontText->CharGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));

	// Écrire le nouveau texte
	FontText->CharGraphics->DrawString(String.c_str(), static_cast<int>(String.size()), FontText->FontType.get(), Gdiplus::PointF(0.0f, 0.0f), FontText->BlackBrush.get());

	// Transférer
	Gdiplus::BitmapData bmData;
	const auto& Rect = Gdiplus::Rect(0, 0, FontText->TextWidth, FontText->TextHeight);
	FontText->CharBitmap->LockBits(&Rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);

	auto& pD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();

	pD3DDevice.GetImmediateContext()->UpdateSubresource(FontText->Texture, 0, 0, bmData.Scan0, FontText->TextWidth * 4, 0);

	FontText->CharBitmap->UnlockBits(&bmData);
}



#include "stdafx.h"
#include "SpriteRenderer.h"
#include "MoteurWindows.h"

using namespace DirectX;
using namespace std;

SpriteRenderer::SpriteRenderer(Pitbull::Actor* Parent, std::variant<Texture*, Font*> Sprite, ShaderSprite* Shader, bool BillBoard)
	: Component{ Parent }
	, Shader{ Shader }
	, Sprite{ Sprite }
	, BillBoard{ BillBoard }
{
	auto& pD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();

	ID3D11Resource* pResource;
	ID3D11Texture2D* pTextureInterface = 0;
	if (holds_alternative<Texture*>(Sprite)) {
		get<Texture*>(Sprite)->TextureView->GetResource(&pResource);
	}
	else {
		get<Font*>(Sprite)->pTextureView->GetResource(&pResource);
	}
	pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	D3D11_TEXTURE2D_DESC desc;
	pTextureInterface->GetDesc(&desc);

	PM3D::DXRelacher(pResource);
	PM3D::DXRelacher(pTextureInterface);

	Dimension.x = float(desc.Width);
	Dimension.y = float(desc.Height);

	Dimension.x = Dimension.x * 2.0f / pD3DDevice.GetLargeur();
	Dimension.y = Dimension.y * 2.0f / pD3DDevice.GetHauteur();
}

void SpriteRenderer::SpriteTick(const float& ElapsedTime)
{

	auto& pD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();

	// Obtenir le contexte
	ID3D11DeviceContext* pImmediateContext = pD3DDevice.GetImmediateContext();

	// Choisir la topologie des primitives
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Source des sommets
	const UINT stride = sizeof(SpriteVertex);
	const UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &Shader->pVertexBuffer, &stride, &offset);

	// input layout des sommets
	pImmediateContext->IASetInputLayout(Shader->pVertexLayout);

	// Le sampler state
	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = Shader->pEffet->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, Shader->pSampleState);

	ID3DX11EffectConstantBuffer* pCB = Shader->pEffet->GetConstantBufferByName("param");
	ID3DX11EffectShaderResourceVariable* variableTexture;
	variableTexture = Shader->pEffet->GetVariableByName("textureEntree")->AsShaderResource();

	pD3DDevice.ActiverMelangeAlpha();

	XMMATRIX Position;
	XMMATRIX Scale;

	if (BillBoard) {
		Position = DirectX::XMMatrixTranslationFromVector(XMVECTOR{ ParentActor->Transform.Position.x + Offset.Position.x, ParentActor->Transform.Position.y + Offset.Position.y, ParentActor->Transform.Position.z + Offset.Position.z });
		Scale = DirectX::XMMatrixScaling(Dimension.x * ParentActor->Transform.Scale.x * Offset.Scale.x, Dimension.y * ParentActor->Transform.Scale.y * Offset.Scale.y, 1.f);

		XMMATRIX ViewProj = PM3D::CMoteurWindows::GetInstance().GetMatViewProj();
		XMVECTOR PositionCamera = PM3D::CMoteurWindows::GetInstance().GetPosition();

		float Angle = atan2f(ParentActor->Transform.Position.x - PositionCamera.vector4_f32[0], ParentActor->Transform.Position.z - PositionCamera.vector4_f32[2]);

		XMMATRIX Rotation = DirectX::XMMatrixRotationY(Angle);

		Position = Scale * Rotation * Position * ViewProj;

	}
	else {
		Position = DirectX::XMMatrixTranslationFromVector(XMVECTOR{ ParentActor->Transform.Position.x + Offset.Position.x, ParentActor->Transform.Position.y + Offset.Position.y, 0.0f });
		Scale = DirectX::XMMatrixScaling(Dimension.x * ParentActor->Transform.Scale.x * Offset.Scale.x, Dimension.y * ParentActor->Transform.Scale.y * Offset.Scale.y, 1.f);

		Position = Scale * Position;
	}
	ShaderParams.MatWorldViewProj = DirectX::XMMatrixTranspose(Position);

	pImmediateContext->UpdateSubresource(Shader->pConstantBuffer, 0, nullptr, &ShaderParams, 0, 0);
	
	pCB->SetConstantBuffer(Shader->pConstantBuffer);

	// Activation de la texture
	if (holds_alternative<Texture*>(Sprite)) {
		variableTexture->SetResource(get<Texture*>(Sprite)->TextureView);
	}
	else {
		variableTexture->SetResource(get<Font*>(Sprite)->pTextureView);
	}

	Shader->pPasse->Apply(0, pImmediateContext);

	// **** Rendu de l'objet
	pImmediateContext->Draw(6, 0);

	pD3DDevice.DesactiverMelangeAlpha();
}

void SpriteRenderer::Write(const std::wstring& s)
{
	// Effacer
	get<Font*>(Sprite)->pCharGraphics->Clear(Gdiplus::Color(0, 0, 0, 0));

	// Écrire le nouveau texte
	get<Font*>(Sprite)->pCharGraphics->DrawString(s.c_str(), static_cast<int>(s.size()), get<Font*>(Sprite)->pFont.get(), Gdiplus::PointF(0.0f, 0.0f), get<Font*>(Sprite)->pBlackBrush.get());

	// Transférer
	Gdiplus::BitmapData bmData;
	const auto& Rect = Gdiplus::Rect(0, 0, get<Font*>(Sprite)->TexWidth, get<Font*>(Sprite)->TexHeight);
	get<Font*>(Sprite)->pCharBitmap->LockBits(&Rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmData);

	auto& pD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();

	pD3DDevice.GetImmediateContext()->UpdateSubresource(get<Font*>(Sprite)->pTexture, 0, 0, bmData.Scan0, get<Font*>(Sprite)->TexWidth * 4, 0);

	get<Font*>(Sprite)->pCharBitmap->UnlockBits(&bmData);
}


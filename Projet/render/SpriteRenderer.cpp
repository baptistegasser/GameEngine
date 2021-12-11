#include "stdafx.h"
#include "SpriteRenderer.h"
#include "MoteurWindows.h"

using namespace DirectX;

SpriteRenderer::SpriteRenderer(Pitbull::Actor* Parent, Texture* Sprite, ShaderSprite* Shader, bool BillBoard)
	: SpriteRenderer{ Parent, Sprite->TextureView, Shader, BillBoard }
{}

SpriteRenderer::SpriteRenderer(Pitbull::Actor* Parent, ID3D11ShaderResourceView* TextureView, ShaderSprite* Shader, bool BillBoard)
	: Component{ Parent }
	, Shader{ Shader }
	, TextureView{ TextureView }
	, BillBoard{ BillBoard }
{
	auto& pD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();

	ID3D11Resource* pResource;
	ID3D11Texture2D* pTextureInterface = 0;

	TextureView->GetResource(&pResource);

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

	ID3D11DeviceContext* pImmediateContext = pD3DDevice.GetImmediateContext();

	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const UINT stride = sizeof(SpriteVertex);
	const UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &Shader->VertexBuffer, &stride, &offset);

	pImmediateContext->IASetInputLayout(Shader->VertexLayout);

	ID3DX11EffectSamplerVariable* variableSampler;
	variableSampler = Shader->Effet->GetVariableByName("SampleState")->AsSampler();
	variableSampler->SetSampler(0, Shader->SampleState);

	ID3DX11EffectConstantBuffer* pCB = Shader->Effet->GetConstantBufferByName("param");
	ID3DX11EffectShaderResourceVariable* variableTexture;
	variableTexture = Shader->Effet->GetVariableByName("textureEntree")->AsShaderResource();

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
		Position = DirectX::XMMatrixTranslationFromVector(XMVECTOR{ Offset.Position.x, Offset.Position.y, 0.0f });
		Scale = DirectX::XMMatrixScaling(Dimension.x * Offset.Scale.x, Dimension.y * Offset.Scale.y, 1.f);

		Position = Scale * Position;
	}
	ShaderParams.MatWorldViewProj = DirectX::XMMatrixTranspose(Position);

	pImmediateContext->UpdateSubresource(Shader->ConstantBuffer, 0, nullptr, &ShaderParams, 0, 0);
	
	pCB->SetConstantBuffer(Shader->ConstantBuffer);

	variableTexture->SetResource(TextureView);

	Shader->Passe->Apply(0, pImmediateContext);

	pImmediateContext->Draw(6, 0);

	pD3DDevice.DesactiverMelangeAlpha();
}

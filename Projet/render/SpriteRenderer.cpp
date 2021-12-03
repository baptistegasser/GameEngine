#include "stdafx.h"
#include "SpriteRenderer.h"
#include "MoteurWindows.h"

using namespace DirectX;

SpriteRenderer::SpriteRenderer(Pitbull::Actor* Parent, Texture* TextureSprite, ShaderSprite* Shader, bool BillBoard)
	: Component{ Parent }
	, TextureSprite{TextureSprite}
	, Shader{Shader}
	, BillBoard{BillBoard}
{
	ID3D11Resource* pResource;
	ID3D11Texture2D* pTextureInterface = 0;
	TextureSprite->TextureView->GetResource(&pResource);
	pResource->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	D3D11_TEXTURE2D_DESC desc;
	pTextureInterface->GetDesc(&desc);

	PM3D::DXRelacher(pResource);
	PM3D::DXRelacher(pTextureInterface);

	Dimension.x = float(desc.Width);
	Dimension.y = float(desc.Height);

	auto& pD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif();
	
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

	XMMATRIX Position = DirectX::XMMatrixTranslationFromVector(XMVECTOR{ ParentActor->Transform.Position.x + Offset.Position.x, ParentActor->Transform.Position.y + Offset.Position.y, ParentActor->Transform.Position.z  + Offset.Position.z});
	XMMATRIX Scale = DirectX::XMMatrixScaling(Dimension.x * ParentActor->Transform.Scale.x * Offset.Scale.x, Dimension.y * ParentActor->Transform.Scale.y * Offset.Scale.y, 1.f);
	if (BillBoard) {
		XMMATRIX ViewProj = PM3D::CMoteurWindows::GetInstance().GetMatViewProj();
		XMVECTOR PositionCamera = PM3D::CMoteurWindows::GetInstance().GetPosition();

		float Angle = atan2f(ParentActor->Transform.Position.x - PositionCamera.vector4_f32[0], ParentActor->Transform.Position.z - PositionCamera.vector4_f32[2]);

		XMMATRIX Rotation = DirectX::XMMatrixRotationY(Angle);

		Position = Scale * Rotation * Position * ViewProj;

	}
	else {
		Position = Scale * Position;
	}
	ShaderParams.MatWorldViewProj = DirectX::XMMatrixTranspose(Position);

	pImmediateContext->UpdateSubresource(Shader->pConstantBuffer, 0, nullptr, &ShaderParams, 0, 0);
	
	pCB->SetConstantBuffer(Shader->pConstantBuffer);

	// Activation de la texture
	variableTexture->SetResource(TextureSprite->TextureView);

	Shader->pPasse->Apply(0, pImmediateContext);

	// **** Rendu de l'objet
	pImmediateContext->Draw(6, 0);

	pD3DDevice.DesactiverMelangeAlpha();
}



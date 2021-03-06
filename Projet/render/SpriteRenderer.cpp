#include "stdafx.h"
#include "SpriteRenderer.h"
#include "EngineD3D11.h"

using namespace DirectX;

SpriteRenderer::SpriteRenderer(Pitbull::Actor* Parent, Texture* Sprite, ShaderSprite* Shader, bool BillBoard)
	: SpriteRenderer{ Parent, Shader, BillBoard }
{
	TextureView = Sprite->TextureView;

	const auto& D3DDevice = EngineD3D11::GetInstance().Device;

	ID3D11Resource* Resource;
	ID3D11Texture2D* TextureInterface;

	TextureView->GetResource(&Resource);

	Resource->QueryInterface<ID3D11Texture2D>(&TextureInterface);
	D3D11_TEXTURE2D_DESC desc;
	TextureInterface->GetDesc(&desc);

	DX_RELEASE(Resource);
	DX_RELEASE(TextureInterface);

	Dimension.x = static_cast<float>(desc.Width) * 2.0f / D3DDevice->ScreenWidth;
	Dimension.y = static_cast<float>(desc.Height) * 2.0f / D3DDevice->ScreenHeight;
}

SpriteRenderer::SpriteRenderer(Pitbull::Actor* Parent, ShaderSprite* Shader, bool BillBoard)
	: Component{ Parent }
	, Shader{ Shader }
	, BillBoard{ BillBoard }
{}

void SpriteRenderer::SpriteTick(const float& ElapsedTime)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& pD3DDevice = Engine.Device;

	ID3D11DeviceContext* pImmediateContext = pD3DDevice->ImmediateContext;

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

	pD3DDevice->ActivateAlphaBlending();
	pD3DDevice->DeactivateZBuffer();

	XMMATRIX Position;
	XMMATRIX Scale;

	if (BillBoard) {
		Position = XMMatrixTranslationFromVector(ParentActor->Transform.Position.ToXMVector() + Offset.Position.ToXMVector());
		Scale = XMMatrixScaling(
			Dimension.x * ParentActor->Transform.Scale.x * Offset.Scale.x,
			Dimension.y * ParentActor->Transform.Scale.y * Offset.Scale.y, 1.f);

		XMMATRIX ViewProj = Engine.MatViewProj;
		XMVECTOR PositionCamera = Engine.GetScene().GetCurrentCamera().GetPosition();

		float Angle = atan2f(ParentActor->Transform.Position.x - PositionCamera.vector4_f32[0], ParentActor->Transform.Position.z - PositionCamera.vector4_f32[2]);

		XMMATRIX Rotation = XMMatrixRotationY(Angle);

		Position = Scale * Rotation * Position * ViewProj;

	}
	else {
		Position = XMMatrixTranslationFromVector(XMVECTOR{ Offset.Position.x, Offset.Position.y, 0.0f });
		Scale = XMMatrixScaling(Dimension.x * Offset.Scale.x, Dimension.y * Offset.Scale.y, 1.f);

		XMMATRIX Rotation = XMMatrixRotationZ(Offset.Rotation.ToEuler().z * XM_PI / 180);

		Position = Rotation * Scale * Position;
	}
	ShaderParams.MatWorldViewProj = XMMatrixTranspose(Position);

	pImmediateContext->UpdateSubresource(Shader->ConstantBuffer, 0, nullptr, &ShaderParams, 0, 0);
	
	pCB->SetConstantBuffer(Shader->ConstantBuffer);

	variableTexture->SetResource(TextureView);

	Shader->Passe->Apply(0, pImmediateContext);

	pImmediateContext->Draw(6, 0);

	pD3DDevice->DeactivateAlphaBlending();
	pD3DDevice->ActivateZBuffer();
}

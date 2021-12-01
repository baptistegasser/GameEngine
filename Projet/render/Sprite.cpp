#include "stdafx.h"
#include "Sprite.h"


Sprite::Sprite(Pitbull::Actor* Parent, Texture* TextureSprite, const bool& BillBoard)
	: Component{ Parent }
	, BillBoard(BillBoard)
	, TextureSprite{TextureSprite}
{
}

void Sprite::SpriteTick(const float& ElapsedTime)
{

//	ID3D11Device* PD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif().GetD3DDevice();
//
//	// Choisir la topologie des primitives
//	pImmediateContext->IASetPrimitiveTopology(
//		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	// Source des sommets
//	const UINT stride = sizeof(SpriteVertex);
//	const UINT offset = 0;
//	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride,
//		&offset);
//
//	// input layout des sommets
//	pImmediateContext->IASetInputLayout(pVertexLayout);
//
//	// Le sampler state
//	ID3DX11EffectSamplerVariable* variableSampler;
//	variableSampler = pEffet->GetVariableByName("SampleState")->AsSampler();
//	variableSampler->SetSampler(0, pSampleState);
//
//	ID3DX11EffectConstantBuffer* pCB =
//		pEffet->GetConstantBufferByName("param");
//	ID3DX11EffectShaderResourceVariable* variableTexture;
//	variableTexture =
//		pEffet->GetVariableByName("textureEntree")->AsShaderResource();
//
//	pDispositif->ActiverMelangeAlpha();
//
//	// Faire le rendu de tous nos sprites
//	for (auto& sprite : tabSprites)
//	{
//		// Initialiser et sélectionner les «constantes» de l'effet
//		ShadersParams sp;
//		sp.matWVP = XMMatrixTranspose(sprite->matPosDim);
//		pImmediateContext->UpdateSubresource(pConstantBuffer, 0, nullptr,
//			&sp, 0, 0);
//
//		pCB->SetConstantBuffer(pConstantBuffer);
//
//		// Activation de la texture
//		variableTexture->SetResource(sprite->Texture->TextureView);
//
//		pPasse->Apply(0, pImmediateContext);
//
//		// **** Rendu de l'objet
//		pImmediateContext->Draw(6, 0);
//	}
//
//	pDispositif->DesactiverMelangeAlpha();
}

#pragma once
#include "stdafx.h"
#include "core/Actor.h"
#include "d3dx11effect.h"
#include "Sprite.h"


class SpriteRenderer : public Pitbull::Actor
{
public:
	SpriteRenderer();
	//virtual ~SpriteRenderer();


	//void SpriteTick(const float ElapsedTime) override;

	//void AjouterSprite(const wchar_t* FileName, int _x, int _y, int _dx = 0, int _dy = 0);
	//void AjouterPanneau(const std::string& NomTexture, const XMFLOAT3& _position,
		//float _dx = 0.0f, float _dy = 0.0f);
	//void AjouterSpriteTexte(ID3D11ShaderResourceView* pTexture, int _x, int _y);


	static SpriteVertex sommets[6];
	ID3D11Buffer* pVertexBuffer;

	ID3D11Buffer* pConstantBuffer;
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique;
	ID3DX11EffectPass* pPasse;
	ID3D11InputLayout* pVertexLayout;

	ID3D11SamplerState* pSampleState;

	void InitEffet();
};
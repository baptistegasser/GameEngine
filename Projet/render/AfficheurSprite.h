#pragma once
#include "stdafx.h"
#include "d3dx11effect.h"
#include "core/Actor.h"

#include "util/ResourcesManager.h"

using namespace DirectX;

namespace PM3D
{

class CDispositifD3D11;

class SpriteVertex
{
public:
	SpriteVertex() = default;
	SpriteVertex(const XMFLOAT3& position, const XMFLOAT2& coordTex)
		: m_Position(position)
		, m_CoordTex(coordTex)
	{
	}

public:
	static UINT numElements;
	static D3D11_INPUT_ELEMENT_DESC layout[];

	XMFLOAT3 m_Position;
	XMFLOAT2 m_CoordTex;
};

class CAfficheurSprite : public Pitbull::Actor
{
public:
	CAfficheurSprite(CDispositifD3D11* _pDispositif);
	virtual ~CAfficheurSprite();
	void SpriteTick(const float ElapsedTime) override;

	void AjouterSprite(const wchar_t* FileName, int _x, int _y, int _dx = 0, int _dy = 0);
	void AjouterPanneau(const wchar_t* FileName, const XMFLOAT3& _position,
		float _dx = 0.0f, float _dy = 0.0f);
	//void AjouterSpriteTexte(ID3D11ShaderResourceView* pTexture, int _x, int _y);

private:

	// TO DO enlveer cette merde
	ResourcesManager ResourcesManager;


	class CSprite
	{
	public:
		//ID3D11ShaderResourceView * pTextureD3D;
		Texture* Texture;

		XMMATRIX matPosDim;
		bool bPanneau;
		CSprite()
			: bPanneau(false)
			//, pTextureD3D(nullptr)
			, Texture{nullptr}
		{
		}
	};

	class CPanneau : public CSprite
	{
	public:
		XMFLOAT3 position;
		XMFLOAT2 dimension;

		CPanneau()
		{
			bPanneau = true;
		}
	};

	static SpriteVertex sommets[6];
	ID3D11Buffer* pVertexBuffer;
	CDispositifD3D11* pDispositif;

	ID3D11Buffer* pConstantBuffer;
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique;
	ID3DX11EffectPass* pPasse;
	ID3D11InputLayout* pVertexLayout;

	ID3D11SamplerState* pSampleState;

	// Tous nos sprites
	std::vector<std::unique_ptr<CSprite>> tabSprites;

	void InitEffet();
};

} // namespace PM3D

#pragma once
#include "stdafx.h"
#include "core/Component.h"

using namespace DirectX;


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

struct ShaderParamsSprite {
	DirectX::XMMATRIX MatWorldViewProj;
};

struct ShaderSprite {
	const wchar_t* FileName;

	//ID3D11Buffer* pVertexBuffer;
	//ID3D11Buffer* pConstantBuffer;
	//ID3DX11Effect* pEffet;
	//ID3DX11EffectTechnique* pTechnique;
	//ID3DX11EffectPass* pPasse;
	//ID3D11InputLayout* pVertexLayout;

	//ID3D11SamplerState* pSampleState;

	static SpriteVertex sommets[6];

	ShaderSprite(const wchar_t* FileName);
};







//
//
//	class Sprite : public Pitbull::Component
//	{
//	public:
//		Texture* TextureSprite;
//		XMMATRIX matPosDim;
//		bool BillBoard;
//
//		Sprite(Pitbull::Actor* Parent, Texture* TextureSprite, const bool& BillBoard);
//		~Sprite() = default;
//
//		void SpriteTick(const float& ElapsedTime);
//
//	};
//

#pragma once
#include "d3dx11effect.h"

class SpriteVertex
{
public:
	SpriteVertex(const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT2& CoordTex);
	static UINT NumElements;
	static D3D11_INPUT_ELEMENT_DESC Layout[];

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 CoordTex;
};

struct ShaderParamsSprite {
	DirectX::XMMATRIX MatWorldViewProj;
};

struct ShaderSprite {
	const wchar_t* FileName;

	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pConstantBuffer;
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique;
	ID3DX11EffectPass* pPasse;
	ID3D11InputLayout* pVertexLayout;

	ID3D11SamplerState* pSampleState;

	static SpriteVertex sommets[6];

	ShaderSprite(const wchar_t* FileName);
	~ShaderSprite();
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

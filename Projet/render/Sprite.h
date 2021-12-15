#pragma once
#include "d3dx11effect.h"

/// <summary>
/// The vertex of a sprite
/// </summary>
class SpriteVertex
{
public:
	SpriteVertex(const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT2& CoordTex);
	static UINT NumElements;
	static D3D11_INPUT_ELEMENT_DESC Layout[];

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 CoordTex;
};

/// <summary>
/// The parameters of the shader of a sprite
/// </summary>
struct ShaderParamsSprite {
	DirectX::XMMATRIX MatWorldViewProj;
};

/// <summary>
/// The shader of a sprite
/// </summary>
struct ShaderSprite {
	const wchar_t* FileName;

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* ConstantBuffer;
	ID3DX11Effect* Effet;
	ID3DX11EffectTechnique* Technique;
	ID3DX11EffectPass* Passe;
	ID3D11InputLayout* VertexLayout;

	ID3D11SamplerState* SampleState;

	static SpriteVertex Vertices[6];

	ShaderSprite(const wchar_t* FileName);
	~ShaderSprite();
};

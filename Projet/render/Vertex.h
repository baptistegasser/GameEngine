#pragma once
using namespace DirectX;

struct Vertex
{
	Vertex() = default;
	Vertex(const XMFLOAT3& Position, const XMFLOAT3& Normal, const XMFLOAT2& TexCoord = XMFLOAT2(0.0f, 0.0f));

	static UINT LayoutCount;
	static D3D11_INPUT_ELEMENT_DESC Layout[];
	
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
};

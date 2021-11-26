#include "stdafx.h"

#pragma once

struct Vertex
{
	Vertex() = default;
	Vertex(const DirectX::XMFLOAT3& Position,
		const DirectX::XMFLOAT3& Normal,
		const DirectX::XMFLOAT2& TexCoord = DirectX::XMFLOAT2(0.0f, 0.0f));

	static UINT LayoutCount;
	static D3D11_INPUT_ELEMENT_DESC Layout[];
	
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
};

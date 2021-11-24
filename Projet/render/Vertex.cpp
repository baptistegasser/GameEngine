#include "stdafx.h"
#include "Vertex.h"

// Definir l'organisation de notre sommet
D3D11_INPUT_ELEMENT_DESC Vertex::Layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

UINT Vertex::LayoutCount = ARRAYSIZE(Vertex::Layout);

Vertex::Vertex(const XMFLOAT3& Position, const XMFLOAT3& Normal, const XMFLOAT2& TexCoord /*= XMFLOAT2(0.0f, 0.0f)*/)
	: Position(Position)
	, Normal(Normal)
	, TexCoord(TexCoord)

{
}

#pragma once

#include "render/Material.h"

struct ObjectMesh
{
	ObjectMesh(const wchar_t* MeshName);

	ID3D11Buffer* PVertexBuffer;
	ID3D11Buffer* PIndexBuffer;
	int SubsetCount;
	std::vector<int> SubsetMaterialIndex;
	std::vector<int> SubsetIndex;
	std::vector<Material> Materials;
};

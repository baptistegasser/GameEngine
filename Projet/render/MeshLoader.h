#pragma once

#include "Material.h"

#include <string>
#include <vector>

class OMBMeshLoader {
public:
	void Load(const std::string& FileName);

	ID3D11Buffer* PVertexBuffer;
	ID3D11Buffer* PIndexBuffer;
	int SubsetCount;
	std::vector<int> SubsetMaterialIndex;
	std::vector<int> SubsetIndex;
	std::vector<Material> Materials;

private:
	struct RawMaterialBlock
	{
		char NomFichierTexture[200];
		char NomMateriau[60];
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		float Puissance;
		bool transparent;

		Material ToMat() const
		{
			Material Mat;
			Mat.TextureFile = NomFichierTexture;
			Mat.Name = NomMateriau;
			Mat.Ambiante = Ambient;
			Mat.Roughness = Diffuse;
			Mat.Specular = Specular;
			Mat.Power = Puissance;
			Mat.Transparency = transparent;
			return Mat;
		}
	};
};

#pragma once

#include "Material.h"
#include "ObjectMesh.h"

class OMBMeshLoader {
public:
	static ObjectMesh Load(const wchar_t* FileName, ObjectMesh& Mesh);

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

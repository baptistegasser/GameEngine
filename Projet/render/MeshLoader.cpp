#include "stdafx.h"
#include "MeshLoader.h"

#include "MoteurWindows.h"
#include "util/Util.h"
#include "resources/resource.h"

void OMBMeshLoader::Load(const wchar_t* FileName, ObjectMesh& Mesh)
{
	ID3D11Device* PD3DDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif().GetD3DDevice();

	std::ifstream fichier;
	fichier.open(FileName, std::ios::in | std::ios_base::binary);
	assert(fichier.is_open());

	// Read vertices
	{
		int32_t nombreSommets;
		fichier.read((char*)&nombreSommets, sizeof(nombreSommets));

		std::unique_ptr<PM3D::CSommetBloc[]> ts(new PM3D::CSommetBloc[nombreSommets]);

		// 1. SOMMETS b) Lecture des sommets à partir d'un fichier binaire
		fichier.read((char*)ts.get(), nombreSommets * sizeof(PM3D::CSommetBloc));

		// 1. SOMMETS b) Création du vertex buffer et copie des sommets
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(PM3D::CSommetBloc) * nombreSommets;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = ts.get();
		Mesh.PVertexBuffer = nullptr;

		PM3D::DXEssayer(PD3DDevice->CreateBuffer(&bd, &InitData, &Mesh.PVertexBuffer), DXE_CREATIONVERTEXBUFFER);
	}

	// 2. INDEX 
	{
		int32_t nombreIndex;
		fichier.read((char*)&nombreIndex, sizeof(nombreIndex));

		std::unique_ptr<uint32_t[]> index(new uint32_t[nombreIndex]);
		fichier.read((char*)index.get(), nombreIndex * sizeof(uint32_t));

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(uint32_t) * nombreIndex;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = index.get();
		Mesh.PIndexBuffer = nullptr;

		PM3D::DXEssayer(PD3DDevice->CreateBuffer(&bd, &InitData, &Mesh.PIndexBuffer), DXE_CREATIONINDEXBUFFER);
	}

	// 3. Les sous-objets
	fichier.read((char*)&Mesh.SubsetCount, sizeof(Mesh.SubsetCount));
	//    Début de chaque sous-objet et un pour la fin
	{
		std::unique_ptr<int32_t[]> si(new int32_t[Mesh.SubsetCount + 1]);

		fichier.read((char*)si.get(), (Mesh.SubsetCount + 1) * sizeof(int32_t));
		Mesh.SubsetIndex.assign(si.get(), si.get() + (Mesh.SubsetCount + 1));
	}

	// 4. MATERIAUX
	// 4a) Créer un matériau de défaut en index 0
	//     Vous pourriez changer les valeurs, j'ai conservé 
	//     celles du constructeur
	int32_t NbMaterial;
	fichier.read((char*)&NbMaterial, sizeof(int32_t));

	Mesh.Materials.resize(NbMaterial);

	RawMaterialBlock mb;
	for (int32_t i = 0; i < NbMaterial; ++i)
	{
		fichier.read((char*)&mb, sizeof(RawMaterialBlock));
		Mesh.Materials[i] = mb.ToMat();
	}

	// 4c) Trouver l'index du materiau pour chaque sous-ensemble
	{
		std::unique_ptr<int32_t[]> smi(new int32_t[Mesh.SubsetCount]);

		fichier.read((char*)smi.get(), (Mesh.SubsetCount) * sizeof(int32_t));
		Mesh.SubsetMaterialIndex.assign(smi.get(), smi.get() + Mesh.SubsetCount);
	}

	// 4d) Chargement des textures
	ResourcesManager& ResourcesManager = PM3D::CMoteurWindows::GetInstance().GetResourcesManager();

	for (uint32_t i = 0; i < Mesh.Materials.size(); ++i)
	{
		if (Mesh.Materials[i].TextureFile.length() > 0)
		{
			std::wstring ws(Mesh.Materials[i].TextureFile.begin(), Mesh.Materials[i].TextureFile.end());
			Mesh.Materials[i].Texture = ResourcesManager.GetTexture(ws);
		}
	}
}

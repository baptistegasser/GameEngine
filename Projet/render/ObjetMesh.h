#pragma once
#include "objet3d.h"
#include "d3dx11effect.h"
#include "chargeur.h"
#include "DeviceD3D11.h"

namespace PM3D
{

class CObjetMesh : public CObjet3D
{
private:
	class CSommetMesh
	{
	public:
		CSommetMesh() = default;
		CSommetMesh(const XMFLOAT3& _position, const XMFLOAT3& _normal, const XMFLOAT2& _coordTex = XMFLOAT2(0, 0))
			: position(_position)
			, normal(_normal)
			, coordTex(_coordTex)
		{
		}

	public:
		static UINT numElements;
		static D3D11_INPUT_ELEMENT_DESC layout[];

		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 coordTex;
	};

	struct MaterialBlock
	{
		char NomFichierTexture[200];
		char NomMateriau[60];
		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
		float Puissance;
		bool transparent;
	};

	class CMaterial
	{
	public:
		std::string NomFichierTexture;
		std::string NomMateriau;
		ID3D11ShaderResourceView* pTextureD3D;

		XMFLOAT4 Ambient;
		XMFLOAT4 Diffuse;
		XMFLOAT4 Specular;
		float Puissance;
		bool transparent;

		CMaterial()
			: pTextureD3D(nullptr)
			, Ambient(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
			, Diffuse(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
			, Specular(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f))
			, Puissance(1.0f)
			, transparent(false)
		{
		}

		void MatToBlock(MaterialBlock& mb)
		{
			strcpy_s(mb.NomFichierTexture, NomFichierTexture.c_str());
			strcpy_s(mb.NomMateriau, NomMateriau.c_str());
			mb.Ambient = Ambient;
			mb.Diffuse = Diffuse;
			mb.Specular = Specular;
			mb.Puissance = Puissance;
			mb.transparent = transparent;

		}

		void BlockToMat(MaterialBlock& mb)
		{
			NomFichierTexture.append(mb.NomFichierTexture);
			NomMateriau.append(mb.NomMateriau);
			Ambient = mb.Ambient;
			Diffuse = mb.Diffuse;
			Specular = mb.Specular;
			Puissance = mb.Puissance;
			transparent = mb.transparent;
		}
	};


public:
	CObjetMesh(const IChargeur& chargeur, DeviceD3D11* pDispositif);
	CObjetMesh(const IChargeur& chargeur, const std::string& nomfichier, DeviceD3D11* _pDispositif);
	CObjetMesh(const std::string& nomfichier, DeviceD3D11* _pDispositif);

	virtual ~CObjetMesh();

	virtual void Anime(float tempsEcoule) override;
	virtual void Draw() override;

protected:
	// **** Fonctions
	void TransfertObjet(const IChargeur& chargeur);
	void InitEffet();
	void EcrireFichierBinaire(const IChargeur& chargeur, const std::string& nomFichier);
	void LireFichierBinaire(const std::string& nomFichier);

protected:
	// ****  Données membres
	XMMATRIX matWorld;				// Matrice de transformation dans le monde
	float rotation;

	// Pour le dessin
	DeviceD3D11* pDispositif;		// On prend en note le dispositif

	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;

	// Les sous-objets
	int NombreSubset;				// Nombre de sous-objets dans le mesh
	std::vector<int> SubsetMaterialIndex;// Index des matériaux
	std::vector<int> SubsetIndex;		// Index des sous-objets

	std::vector<CMaterial> Material;		// Vecteur des matériaux

	// Pour les effets et shaders
	ID3D11SamplerState* pSampleState;
	ID3D11Buffer* pConstantBuffer;
	ID3DX11Effect* pEffet;
	ID3DX11EffectTechnique* pTechnique;
	ID3DX11EffectPass* pPasse;
	ID3D11InputLayout* pVertexLayout;
};

} // namespace PM3D

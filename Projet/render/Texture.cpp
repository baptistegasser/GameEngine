#include "stdafx.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "dispositifD3D11.h"
#include "MoteurWindows.h"
#include "resources/resource.h"
#include "util/Util.h"

using namespace DirectX;

Texture::~Texture()
{
	DX_RELEASE(TextureResource);
	DX_RELEASE(TextureView);
}

Texture::Texture(const std::wstring& filename)
	: Filename(filename)
	, TextureView(nullptr)
{
	ID3D11Device* pDevice = PM3D::CMoteurWindows::GetInstance().GetDispositif().GetD3DDevice();

	// Charger la texture en ressource
	DX_TRY(CreateDDSTextureFromFile(pDevice,
		Filename.c_str(),
		&TextureResource,
		&TextureView), DXE_FICHIERTEXTUREINTROUVABLE);
}

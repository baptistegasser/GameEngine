#include "stdafx.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "DeviceD3D11.h"
#include "EngineD3D11.h"
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
	ID3D11Device* Device = EngineD3D11::GetInstance().Device->D3DDevice;

	// Charger la texture en ressource
	DX_TRY(CreateDDSTextureFromFile(Device,
		Filename.c_str(),
		&TextureResource,
		&TextureView), DXE_FICHIERTEXTUREINTROUVABLE);
}

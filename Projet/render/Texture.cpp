#include "StdAfx.h"
#include "strsafe.h"
#include "dispositifD3D11.h"
#include "Texture.h"
#include "resources/resource.h"
#include "util/util.h"
#include "DDSTextureLoader.h"
#include "MoteurWindows.h"

using namespace DirectX;

namespace PM3D
{

CTexture::~CTexture()
{
	DXRelacher(m_TexturResource);
	DXRelacher(m_Texture);
}

const std::wstring& CTexture::GetFilename() const
{
	return m_Filename;
}

ID3D11ShaderResourceView* CTexture::GetD3DTexture()
{
	return m_Texture;
}

CTexture::CTexture(const std::wstring& filename)
	: m_Filename(filename)
	, m_Texture(nullptr)
{
	ID3D11Device* pDevice = CMoteurWindows::GetInstance().GetDispositif().GetD3DDevice();

	// Charger la texture en ressource
	DXEssayer(CreateDDSTextureFromFile(pDevice,
		m_Filename.c_str(),
		&m_TexturResource,
		&m_Texture), DXE_FICHIERTEXTUREINTROUVABLE);
}

} // namespace PM3D

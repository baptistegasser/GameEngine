#pragma once

namespace PM3D
{

class CDispositifD3D11;

class CTexture
{
public:
	CTexture(const std::wstring& filename);
	~CTexture();

	const std::wstring& GetFilename() const;
	ID3D11ShaderResourceView* GetD3DTexture();

private:
	std::wstring m_Filename;
	ID3D11Resource* m_TexturResource;
	ID3D11ShaderResourceView* m_Texture;
};

} // namespace PM3D

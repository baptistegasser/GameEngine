#pragma once

struct Texture
{
	Texture(const std::wstring& filename);
	~Texture();

	std::wstring Filename;
	ID3D11Resource* TextureResource;
	ID3D11ShaderResourceView* TextureView;
};

#pragma once

#include "Texture.h"

#include <string>

struct Material {
	std::string Name;
	std::string TextureFile;
	DirectX::XMFLOAT4 BaseColor;
	DirectX::XMFLOAT4 Ambiante;
	DirectX::XMFLOAT4 Roughness;
	DirectX::XMFLOAT4 Specular;
	float Power;
	bool Transparency;

	PM3D::CTexture* Texture;
};

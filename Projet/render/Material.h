#pragma once

#include "Texture.h"

struct Material {
	DirectX::XMVECTOR BaseColor;
	DirectX::XMVECTOR Ambiante;
	DirectX::XMVECTOR Roughness;
	DirectX::XMVECTOR Specular;

	PM3D::CTexture* Texture;
};

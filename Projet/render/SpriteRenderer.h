#pragma once
#include "stdafx.h"
#include "core/Component.h"
#include "d3dx11effect.h"
#include "render/Texture.h"
#include "render/Sprite.h"
#include "math/Transform.h"

class SpriteRenderer : public Pitbull::Component {


public :
	SpriteRenderer(Pitbull::Actor* Parent, Texture* TextureSprite, ShaderSprite* Shader, bool BillBoard);

	void SpriteTick(const float& ElapsedTime) override;

	ShaderParamsSprite ShaderParams;
	Math::Transform Offset;

private :
	
	bool BillBoard;
	Texture* TextureSprite;
	ShaderSprite* Shader;
	DirectX::XMFLOAT2 Dimension;
};
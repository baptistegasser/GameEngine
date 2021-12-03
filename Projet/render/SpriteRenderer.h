#pragma once
#include "stdafx.h"
#include "core/Actor.h"
#include "d3dx11effect.h"
#include "render/Texture.h"
#include "render/Sprite.h"

class SpriteRenderer : public Pitbull::Actor {


public :
	SpriteRenderer(Texture* TextureSprite, ShaderSprite* Shader, bool BillBoard);

	void SpriteTick(const float ElapsedTime) override;

	ShaderParamsSprite ShaderParams;

private :

	bool BillBoard;
	Texture* TextureSprite;
	ShaderSprite* Shader;
	DirectX::XMFLOAT2 Dimension;
};
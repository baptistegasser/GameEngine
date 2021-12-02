#pragma once
#include "stdafx.h"
#include "core/Actor.h"
#include "d3dx11effect.h"
#include "render/Texture.h"

class SpriteRenderer : public Pitbull::Actor {


	SpriteRenderer(Texture* Texture);
	
	
	//virtual ~SpriteRenderer();

	//void SpriteTick(const float ElapsedTime) override;
};
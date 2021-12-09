#pragma once
#include "stdafx.h"
#include "core/Component.h"
#include "d3dx11effect.h"
#include "render/Texture.h"
#include "render/Sprite.h"
#include "math/Transform.h"
#include "render/Font.h"
#include <variant>

class SpriteRenderer : public Pitbull::Component {


public :
	SpriteRenderer(Pitbull::Actor* Parent, std::variant<Texture*,Font*> Sprite, ShaderSprite* Shader, bool BillBoard);

	void SpriteTick(const float& ElapsedTime) override;

	void Write(const std::wstring& s);

	ShaderParamsSprite ShaderParams;
	Math::Transform Offset;

private :
	
	bool BillBoard;
	ShaderSprite* Shader;
	DirectX::XMFLOAT2 Dimension;
	std::variant<Texture*, Font*> Sprite;
};
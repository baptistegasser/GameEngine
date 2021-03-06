#pragma once
#include "core/Component.h"
#include "d3dx11effect.h"
#include "render/Texture.h"
#include "render/Sprite.h"
#include "math/Transform.h"

class SpriteRenderer : public Pitbull::Component {
public :
	/// <summary>
	/// The constructor for a sprite or a billboard
	/// </summary>
	/// <param name="Parent"> The parent actor </param>
	/// <param name="Sprite"> The sprite's texture </param>
	/// <param name="Shader"> The shader to use </param>
	/// <param name="BillBoard"> True if it is a billboard, false otherwise </param>
	SpriteRenderer(Pitbull::Actor* Parent, Texture* Sprite, ShaderSprite* Shader, bool BillBoard);
	~SpriteRenderer() override = default;

	void SpriteTick(const float& ElapsedTime) override;

	/// <summary>
	/// The offset to use if it a billboard
	/// Otherwise it is the position of the sprite
	/// </summary>
	Math::Transform Offset;

protected:
	SpriteRenderer(Pitbull::Actor* Parent, ShaderSprite* Shader, bool BillBoard);

	ID3D11ShaderResourceView* TextureView;

	/// <summary>
	/// The dimension of the textureView
	/// </summary>
	DirectX::XMFLOAT2 Dimension;

private:
	ShaderParamsSprite ShaderParams{};
	ShaderSprite* Shader;
	bool BillBoard;
};
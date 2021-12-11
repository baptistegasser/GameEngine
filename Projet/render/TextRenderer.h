#pragma once
#include "render/Font.h"
#include "render/SpriteRenderer.h"

class TextRenderer : public SpriteRenderer {

public :
	/// <summary>
	/// The constructor of a TextSprite
	/// </summary>
	/// <param name="Parent"> The parent actor </param>
	/// <param name="Sprite"> The font to use </param>
	/// <param name="Shader"> The shader to use </param>
	TextRenderer(Pitbull::Actor* Parent, Font* Sprite, ShaderSprite* Shader);

	/// <summary>
	/// Write a text on the sprite
	/// </summary>
	/// <param name="String"> The string to write </param>
	void Write(const std::wstring& String);
private :
	Font* FontText;

	/*ID3D11Texture2D* Texture;
	ID3D11ShaderResourceView* TextureView;*/
};
#pragma once
#include "render/Font.h"
#include "render/SpriteRenderer.h"

class TextRenderer : public SpriteRenderer {

public :
	/// <summary>
	/// The constructor of a TextSprite
	/// </summary>
	/// <param name="Parent"> The parent actor </param>
	/// <param name="Font"> The font to use </param>
	/// <param name="Shader"> The shader to use </param>
	TextRenderer(Pitbull::Actor* Parent, Font* Font, ShaderSprite* Shader, int CanvasHeight, int CanvasWidth);
	~TextRenderer() override;

	/// <summary>
	/// Write a text on the sprite
	/// </summary>
	/// <param name="String"> The string to write </param>
	void Write(const std::wstring& String);

	void UpdateCanvas(int NewHeight, int NewWidth);

private:
	std::unique_ptr<Font> FontText;
	int CanvasHeight, CanvasWidth;
	std::wstring Value;

	std::unique_ptr<Gdiplus::Bitmap> CharBitmap;
	std::unique_ptr<Gdiplus::Graphics> CharGraphics;

	ID3D11Texture2D* Texture2D;
};
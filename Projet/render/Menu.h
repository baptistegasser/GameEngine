#pragma once
#include "stdafx.h"
#include <functional>

#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "core/Actor.h"

class Button : public Pitbull::Component
{
public:
	Button(Pitbull::Actor* Parent, TextRenderer* Text, SpriteRenderer* RaisedSprite, SpriteRenderer* PressedSprite, std::function<void()> Cb);
	void Raise();
	void Press();
	void Exec() const;
	void SetOffset(float Text, float Raised, float Texture);
private:
	void SpriteTick(const float& ElapsedTime) override;
	SpriteRenderer* Current;
	TextRenderer* Text;
	SpriteRenderer* RaisedSprite;
	SpriteRenderer* PressedSprite;
	std::function<void()> Cb;
	float RaisedOffset = 0;
};

class Menu : public Pitbull::Actor
{
public:
	Menu(bool IsMainMenu);
	void Tick(const float ElapsedTime) override;
	void SelectPrevious();
	void SelectNext();
};

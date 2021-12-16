#pragma once
#include "stdafx.h"
#include <functional>

#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "core/Actor.h"

class Button : public Pitbull::Component
{
public:
	Button(Pitbull::Actor* Parent, TextRenderer* Text, SpriteRenderer* RaisedSprite, SpriteRenderer* PressedSprite, SpriteRenderer* BallSprite, std::function<void()> Cb);
	void Select();
	void Deselect();
	void Exec() const;
	void SetOffset(float Text, float Raised, float Texture, float Ball);
private:
	void SpriteTick(const float& ElapsedTime) override;
	SpriteRenderer* Current;
	TextRenderer* Text;
	SpriteRenderer* RaisedSprite;
	SpriteRenderer* PressedSprite;
	SpriteRenderer* BallSprite;
	bool Selected = false;
	std::function<void()> Cb;
	float RaisedOffset = 0;
};

class Menu : public Pitbull::Actor
{
public:
	Menu(bool IsMainMenu);
	void UITick(const float ElapsedTime) override;
private:
	bool ProcessInputs();
	void Deselect(size_t Index);
	void Select(size_t Index);
	void SelectPrevious();
	void SelectNext();
	float Accumulator;
	size_t CurrentlySelected = 0;
	std::vector<Button*> Buttons;
};

#pragma once
#include "stdafx.h"
#include <functional>

#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Timer.h"
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
	std::unique_ptr<TextRenderer> Text;
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
	Menu();
	void UITick(const float ElapsedTime) override;
protected:
	virtual Button* CreateTopButton() = 0;
	virtual Button* CreateBottomButton() = 0;
	virtual void CreateBackdrop() = 0;
	virtual void CreateTitle() = 0;
	void Init();

	Font* Font1 = new Font{ 48.0f, { 255, 246, 141 } };
	Font* Font2 = new Font{ 64.0f, { 169, 255, 246 } };

	ShaderSprite* SpriteShader;
	Texture* RaisedTexture;
	Texture* PressedTexture;
	Texture* Ball;
private:
	bool ProcessInputs();
	void Deselect(size_t Index) const;
	void Select(size_t Index) const;
	void SelectPrevious();
	void SelectNext();

	float Accumulator{};
	size_t CurrentlySelected = 0;
	std::vector<Button*> Buttons;
};

class MainMenu : public Menu
{
public:
	MainMenu() { Init(); }
protected:
	Button* CreateTopButton() override;
	Button* CreateBottomButton() override;
	void CreateBackdrop() override;
	void CreateTitle() override;
};

class PauseMenu : public Menu
{
public:
	PauseMenu() { Init(); }
protected:
	Button* CreateTopButton() override;
	Button* CreateBottomButton() override;
	void CreateBackdrop() override;
	void CreateTitle() override;
};

class EndGameMenu : public Menu
{
public:
	EndGameMenu(Timer* ATimer) : MyTimer{ ATimer } { Init(); }
protected:
	Button* CreateTopButton() override;
	Button* CreateBottomButton() override;
	void CreateBackdrop() override;
	void CreateTitle() override;
	Timer* MyTimer;
};

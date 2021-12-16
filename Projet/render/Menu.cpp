#include "stdafx.h"
#include "Menu.h"

#include <utility>

#include "render/EngineD3D11.h"
#include "render/TextRenderer.h"
#include "render/SpriteRenderer.h"
#include "render/Font.h"

Button::Button(Pitbull::Actor* Parent, TextRenderer* Text, SpriteRenderer* RaisedSprite, SpriteRenderer* PressedSprite, std::function<void()> Cb)
	: Component{ Parent }
	, Current{ PressedSprite }
	, Text{ Text }
	, RaisedSprite{ RaisedSprite }
	, PressedSprite{ PressedSprite }
	, Cb{ std::move(Cb) }
{}

void Button::Press()
{
	Current = PressedSprite;
	Text->Offset.Position.y += RaisedOffset;
}

void Button::Raise()
{
	Current = RaisedSprite;
	Text->Offset.Position.y -= RaisedOffset;
}

void Button::Exec() const
{
	Cb();
}

void Button::SetOffset(float Text, float Raised, float Texture)
{
	RaisedOffset = Raised;
	this->Text->Offset.Position.y = Text;
	PressedSprite->Offset.Position.y = Texture;
}

void Button::SpriteTick(const float& ElapsedTime)
{
	Component::SpriteTick(ElapsedTime);

	Current->SpriteTick(ElapsedTime);
	Text->SpriteTick(ElapsedTime);
}

Menu::Menu(bool IsMainMenu)
{
	auto& RessourceManager = EngineD3D11::GetInstance().ResourcesManager;

	auto SpriteShader = RessourceManager.GetShaderSprite(L".\\shaders\\sprite1.fx");
	auto Font1 = new Font{ L"Arial Rounded MT Bold", Gdiplus::FontStyleBold, 48.0f, { 255, 246, 141 } };
	auto Font2 = new Font{ L"Arial Rounded MT Bold", Gdiplus::FontStyleBold, 64.0f, { 169, 255, 246 } };

	auto RaisedTexture = RessourceManager.GetTexture(L".\\modeles\\ui\\button.dds");
	auto PressedButton = RessourceManager.GetTexture(L".\\modeles\\ui\\button-pressed.dds");

	const auto RaisedSprite = AddComponent<SpriteRenderer>(
		RaisedTexture, SpriteShader, false);
	const auto PressedSprite = AddComponent<SpriteRenderer>(
		PressedButton, SpriteShader, false);

	const auto RaisedSprite2 = AddComponent<SpriteRenderer>(
		RaisedTexture, SpriteShader, false);
	const auto PressedSprite2 = AddComponent<SpriteRenderer>(
		PressedButton, SpriteShader, false);

	if (IsMainMenu)
	{
		const auto BackDrop = AddComponent<SpriteRenderer>(
			RessourceManager.GetTexture(L".\\modeles\\ui\\mainmenu.dds"), SpriteShader, false);
	}
	else
	{
		const auto BackDrop = AddComponent<SpriteRenderer>(
			RessourceManager.GetTexture(L".\\modeles\\ui\\background50.dds"), SpriteShader, false);
	}

	const auto Background = AddComponent<SpriteRenderer>(
		RessourceManager.GetTexture(L".\\modeles\\ui\\back.dds"), SpriteShader, false);

	TextRenderer* MenuText;
	if (IsMainMenu)
	{
		MenuText = AddComponent<TextRenderer>(Font2, SpriteShader, 100, 550);
		MenuText->Write(L"M A I N    M E N U");
	}
	else
	{
		MenuText = AddComponent<TextRenderer>(Font2, SpriteShader, 100, 300);
		MenuText->Write(L"P A U S E");
	}
	MenuText->Offset.Position.y = 0.39f;

	TextRenderer* PlayText;
	if (IsMainMenu)
	{
		PlayText = new TextRenderer(this, Font1, SpriteShader, 100, 175);
		PlayText->Write(L"P L A Y");
	}
	else
	{
		PlayText = new TextRenderer(this, Font1, SpriteShader, 100, 280);
		PlayText->Write(L"R E S U M E");
	}

	const auto ExitText = new TextRenderer(this, Font1, SpriteShader, 100, 140);
	ExitText->Write(L"E x i t");

	const auto PlayButton = AddComponent<Button>(
		PlayText, RaisedSprite, PressedSprite, [](void) {}
	);
	PlayButton->SetOffset(-0.04f, -0.015f, 0.0f);

	const auto ExitButton = AddComponent<Button>(
		ExitText, RaisedSprite2, PressedSprite2, [](void) {}
	);
	ExitButton->SetOffset(-0.315f, -0.015f, -0.28f);
}

void Menu::Tick(const float ElapsedTime)
{
	Actor::Tick(ElapsedTime);
}

void Menu::SelectPrevious()
{
}

void Menu::SelectNext()
{
}

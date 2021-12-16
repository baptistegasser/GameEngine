#include "stdafx.h"
#include "Menu.h"

#include <utility>

#include "render/EngineD3D11.h"
#include "render/TextRenderer.h"
#include "render/SpriteRenderer.h"
#include "render/Font.h"

Button::Button(Pitbull::Actor* Parent, TextRenderer* Text, SpriteRenderer* RaisedSprite, SpriteRenderer* PressedSprite, SpriteRenderer* BallSprite, std::function<void()> Cb)
	: Component{ Parent }
	, Current{ PressedSprite }
	, Text{ Text }
	, RaisedSprite{ RaisedSprite }
	, PressedSprite{ PressedSprite }
	, BallSprite{ BallSprite }
	, Cb{ std::move(Cb) }
{}

void Button::Deselect()
{
	Selected = false;
	Current = PressedSprite;
	Text->Offset.Position.y += RaisedOffset;
}

void Button::Select()
{
	Selected = true;
	Current = RaisedSprite;
	Text->Offset.Position.y -= RaisedOffset;
}

void Button::Exec() const
{
	Cb();
}

void Button::SetOffset(float Text, float Raised, float Texture, float Ball)
{
	RaisedOffset = Raised;
	this->Text->Offset.Position.y = Text;
	PressedSprite->Offset.Position.y = Texture;
	RaisedSprite->Offset.Position.y = Texture;
	BallSprite->Offset.Scale = BallSprite->Offset.Scale / 20;
	BallSprite->Offset.Position.x = -0.13f;
	BallSprite->Offset.Position.y = Ball;
}

void Button::SpriteTick(const float& ElapsedTime)
{
	Component::SpriteTick(ElapsedTime);

	Current->SpriteTick(ElapsedTime);
	Text->SpriteTick(ElapsedTime);
	if (Selected) BallSprite->SpriteTick(ElapsedTime);
	BallSprite->Offset.RotateZ(-500.0f * ElapsedTime);
}

Menu::Menu(bool IsMainMenu)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto SpriteShader = RessourceManager.GetShaderSprite(L".\\shaders\\sprite1.fx");
	auto Font1 = new Font{ 48.0f, { 255, 246, 141 } };
	auto Font2 = new Font{ 64.0f, { 169, 255, 246 } };

	auto RaisedTexture = RessourceManager.GetTexture(L".\\modeles\\ui\\button.dds");
	auto PressedTexture = RessourceManager.GetTexture(L".\\modeles\\ui\\button-pressed.dds");

	auto Ball = RessourceManager.GetTexture(L".\\modeles\\ui\\ball.dds");

	const auto RaisedSprite = new SpriteRenderer{
		this, RaisedTexture, SpriteShader, false };
	const auto PressedSprite = new SpriteRenderer{
		this, PressedTexture, SpriteShader, false };

	const auto RaisedSprite2 = new SpriteRenderer{
		this, RaisedTexture, SpriteShader, false };
	const auto PressedSprite2 = new SpriteRenderer{
		this, PressedTexture, SpriteShader, false };

	const auto BallSprite = AddComponent<SpriteRenderer>(
		Ball, SpriteShader, false);
	const auto BallSprite2 = AddComponent<SpriteRenderer>(
		Ball, SpriteShader, false);

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
	std::function<void()> PlayFunction;
	if (IsMainMenu)
	{
		PlayText = new TextRenderer(this, Font1, SpriteShader, 100, 175);
		PlayText->Write(L"P L A Y");
		PlayFunction = [&Engine]()
		{
			Engine.RequestLoadLevel();
		};
	}
	else
	{
		PlayText = new TextRenderer(this, Font1, SpriteShader, 100, 220);
		PlayText->Write(L"RESUME");
		PlayFunction = [&Engine, this]()
		{
			Engine.UnPause();
			Active = false;
		};
	}

	const auto ExitText = new TextRenderer(this, Font1, SpriteShader, 100, 140);
	ExitText->Write(L"E x i t");

	const auto PlayButton = AddComponent<Button>(
		PlayText, RaisedSprite, PressedSprite, BallSprite, PlayFunction
	);
	PlayButton->SetOffset(-0.04f, -0.015f, 0.0f, 0.015f);

	const auto ExitButton = AddComponent<Button>(
		ExitText, RaisedSprite2, PressedSprite2, BallSprite2, [&Engine]()
		{
			Engine.Stop();
		}
	);
	ExitButton->SetOffset(-0.315f, -0.015f, -0.28f, -0.26f);

	Buttons.push_back(PlayButton);
	Buttons.push_back(ExitButton);

	Select(CurrentlySelected);
}

void Menu::UITick(const float ElapsedTime)
{
	Actor::Tick(ElapsedTime);

	Accumulator += ElapsedTime;

	if (Accumulator > 0.3f)
	{
		if (ProcessInputs()) Accumulator = 0;
	}
}

bool Menu::ProcessInputs()
{
	const InputManager& InputManager = InputManager::GetInstance();

	if (InputManager.IsKeyPressed(DIK_W)) {
		SelectPrevious();
		return true;
	}
	if (InputManager.IsKeyPressed(DIK_S)) {
		SelectNext();
		return true;
	}
	if (InputManager.IsKeyPressed(DIK_RETURN))
	{
		Buttons[CurrentlySelected]->Exec();
		return true;
	}

	return false;
}

void Menu::Deselect(size_t Index)
{
	Buttons[Index]->Deselect();
}

void Menu::Select(size_t Index)
{
	Buttons[Index]->Select();
}

void Menu::SelectPrevious()
{
	Deselect(CurrentlySelected);
	if (CurrentlySelected > 0) CurrentlySelected--;
	Select(CurrentlySelected);
}

void Menu::SelectNext()
{
	Deselect(CurrentlySelected);
	if (CurrentlySelected < Buttons.size() - 1) CurrentlySelected++;
	Select(CurrentlySelected);
}

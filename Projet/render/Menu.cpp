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
	, Text{ std::unique_ptr<TextRenderer>(Text) }
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

Menu::Menu()
{
	auto& ResourcesManager = EngineD3D11::GetInstance().ResourcesManager;

	SpriteShader = ResourcesManager.GetShaderSprite(L".\\shaders\\sprite1.fx");
	RaisedTexture = ResourcesManager.GetTexture(L".\\modeles\\ui\\button.dds");
	PressedTexture = ResourcesManager.GetTexture(L".\\modeles\\ui\\button-pressed.dds");
	Ball = ResourcesManager.GetTexture(L".\\modeles\\ui\\ball.dds");
}

void Menu::UITick(const float ElapsedTime)
{
	Actor::Tick(ElapsedTime);

	Accumulator += ElapsedTime;

	if (Accumulator > 0.2f)
	{
		if (ProcessInputs()) Accumulator = 0;
	}
}

void Menu::Init()
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& ResourcesManager = Engine.ResourcesManager;

	CreateBackdrop();

	AddComponent<SpriteRenderer>(
		ResourcesManager.GetTexture(L".\\modeles\\ui\\back.dds"), SpriteShader, false);

	CreateTitle();

	Buttons.push_back(CreateTopButton());
	Buttons.push_back(CreateBottomButton());

	Select(CurrentlySelected);
}

bool Menu::ProcessInputs()
{
	const InputManager& InputManager = InputManager::GetInstance();

	if (InputManager.IsKeyPressed(DIK_W) || InputManager.IsKeyPressed(DIK_UP)) {
		SelectPrevious();
		return true;
	}
	if (InputManager.IsKeyPressed(DIK_S) || InputManager.IsKeyPressed(DIK_DOWN)) {
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

void Menu::Deselect(size_t Index) const
{
	Buttons[Index]->Deselect();
}

void Menu::Select(size_t Index) const
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

Button* MainMenu::CreateTopButton()
{
	auto& Engine = EngineD3D11::GetInstance();
	const auto RaisedSprite = new SpriteRenderer{
		this, RaisedTexture, SpriteShader, false };
	const auto PressedSprite = new SpriteRenderer{
		this, PressedTexture, SpriteShader, false };
	const auto BallSprite = AddComponent<SpriteRenderer>(
		Ball, SpriteShader, false);
	const auto PlayText = new TextRenderer(this, Font1, SpriteShader, 100, 175);
	PlayText->Write(L"P L A Y");
	const auto PlayButton = AddComponent<Button>(
		PlayText, RaisedSprite, PressedSprite, BallSprite, [&Engine]()
		{
			Engine.RequestLoadLevel();
		});
	PlayButton->SetOffset(-0.04f, -0.015f, 0.0f, 0.015f);
	return PlayButton;
}

Button* MainMenu::CreateBottomButton()
{
	auto& Engine = EngineD3D11::GetInstance();
	const auto RaisedSprite = new SpriteRenderer{
		this, RaisedTexture, SpriteShader, false };
	const auto PressedSprite = new SpriteRenderer{
		this, PressedTexture, SpriteShader, false };
	const auto BallSprite = AddComponent<SpriteRenderer>(
		Ball, SpriteShader, false);
	const auto ExitText = new TextRenderer(this, Font1, SpriteShader, 100, 140);
	ExitText->Write(L"E x i t");
	const auto ExitButton = AddComponent<Button>(
		ExitText, RaisedSprite, PressedSprite, BallSprite, [&Engine]()
		{
			Engine.Stop();
		}
	);
	ExitButton->SetOffset(-0.315f, -0.015f, -0.28f, -0.26f);
	return ExitButton;
}

void MainMenu::CreateBackdrop()
{
	auto& RessourceManager = EngineD3D11::GetInstance().ResourcesManager;
	AddComponent<SpriteRenderer>(
		RessourceManager.GetTexture(L".\\modeles\\ui\\mainmenu.dds"), SpriteShader, false);
}

void MainMenu::CreateTitle()
{
	const auto MenuText = AddComponent<TextRenderer>(Font2, SpriteShader, 100, 550);
	MenuText->Write(L"M A I N    M E N U");
	MenuText->Offset.Position.y = 0.39f;
}

Button* PauseMenu::CreateTopButton()
{
	auto& Engine = EngineD3D11::GetInstance();
	const auto RaisedSprite = new SpriteRenderer{
		this, RaisedTexture, SpriteShader, false };
	const auto PressedSprite = new SpriteRenderer{
		this, PressedTexture, SpriteShader, false };
	const auto BallSprite = AddComponent<SpriteRenderer>(
		Ball, SpriteShader, false);
	const auto ResumeText = new TextRenderer(this, Font1, SpriteShader, 100, 220);
	ResumeText->Write(L"RESUME");
	const auto PlayButton = AddComponent<Button>(
		ResumeText, RaisedSprite, PressedSprite, BallSprite, [&Engine, this]()
		{
			Engine.UnPause();
			Active = false;
		});
	PlayButton->SetOffset(-0.04f, -0.015f, 0.0f, 0.015f);
	return PlayButton;
}

Button* PauseMenu::CreateBottomButton()
{
	auto& Engine = EngineD3D11::GetInstance();
	const auto RaisedSprite = new SpriteRenderer{
		this, RaisedTexture, SpriteShader, false };
	const auto PressedSprite = new SpriteRenderer{
		this, PressedTexture, SpriteShader, false };
	const auto BallSprite = AddComponent<SpriteRenderer>(
		Ball, SpriteShader, false);
	const auto ExitText = new TextRenderer(this, Font1, SpriteShader, 100, 140);
	ExitText->Write(L"E x i t");
	const auto ExitButton = AddComponent<Button>(
		ExitText, RaisedSprite, PressedSprite, BallSprite, [&Engine]()
		{
			Engine.Stop();
		}
	);
	ExitButton->SetOffset(-0.315f, -0.015f, -0.28f, -0.26f);
	return ExitButton;
}

void PauseMenu::CreateBackdrop()
{
	auto& ResourcesManager = EngineD3D11::GetInstance().ResourcesManager;
	AddComponent<SpriteRenderer>(
		ResourcesManager.GetTexture(L".\\modeles\\ui\\background50.dds"), SpriteShader, false);
}

void PauseMenu::CreateTitle()
{
	const auto MenuText = AddComponent<TextRenderer>(Font2, SpriteShader, 100, 300);
	MenuText->Write(L"P A U S E");
	MenuText->Offset.Position.y = 0.39f;
}

Button* EndGameMenu::CreateTopButton()
{
	auto& Engine = EngineD3D11::GetInstance();
	const auto RaisedSprite = new SpriteRenderer{
		this, RaisedTexture, SpriteShader, false };
	const auto PressedSprite = new SpriteRenderer{
		this, PressedTexture, SpriteShader, false };
	const auto BallSprite = AddComponent<SpriteRenderer>(
		Ball, SpriteShader, false);
	const auto ResumeText = new TextRenderer(this, Font1, SpriteShader, 100, 240);
	ResumeText->Write(L"RESTART");
	const auto PlayButton = AddComponent<Button>(
		ResumeText, RaisedSprite, PressedSprite, BallSprite, [&Engine, this]()
		{
			Engine.RequestLoadLevel();
		});
	PlayButton->SetOffset(-0.04f, -0.015f, 0.0f, 0.015f);
	return PlayButton;
}

Button* EndGameMenu::CreateBottomButton()
{
	auto& Engine = EngineD3D11::GetInstance();
	const auto RaisedSprite = new SpriteRenderer{
		this, RaisedTexture, SpriteShader, false };
	const auto PressedSprite = new SpriteRenderer{
		this, PressedTexture, SpriteShader, false };
	const auto BallSprite = AddComponent<SpriteRenderer>(
		Ball, SpriteShader, false);
	const auto ExitText = new TextRenderer(this, Font1, SpriteShader, 100, 140);
	ExitText->Write(L"E x i t");
	const auto ExitButton = AddComponent<Button>(
		ExitText, RaisedSprite, PressedSprite, BallSprite, [&Engine]()
		{
			Engine.Stop();
		}
	);
	ExitButton->SetOffset(-0.315f, -0.015f, -0.28f, -0.26f);
	return ExitButton;
}

void EndGameMenu::CreateBackdrop()
{
	auto& RessourceManager = EngineD3D11::GetInstance().ResourcesManager;
	AddComponent<SpriteRenderer>(
		RessourceManager.GetTexture(L".\\modeles\\ui\\mainmenu.dds"), SpriteShader, false);
}

void EndGameMenu::CreateTitle()
{
	const auto MenuText = AddComponent<TextRenderer>(Font2, SpriteShader, 100, 600);
	MenuText->Write(MyTimer->GetValue());
	MenuText->Offset.Position.y = 0.39f;
}

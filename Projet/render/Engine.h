#pragma once
#include "Device.h"

#include "core/InputManager.h"
#include "core/Actor.h"
#include "core/Scene.h"
#include "physic/PhysicManager.h"

#include "util/ResourcesManager.h"
#include "util/Singleton.h"

#include "render/EffectManager.h"
#include "gameplay/GameFactory.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <iostream>
#include <chrono>

constexpr int FramesPerSecond = 60;
constexpr double MSPerFrame = 1.0 / static_cast<double>(FramesPerSecond);

template <class T, class TDevice>
class Engine : public Singleton<T>
{
public:
	virtual void Run();
	virtual int Init();

	[[nodiscard]] Scene& GetScene() const noexcept { return *CurrentScene; }

	/// <summary>
	/// Represent the state of the engine.
	/// As state are represent by bit flag, multiple state can be possible at any given time.
	/// </summary>
	enum States
	{
		Initialization = 1 << 0,
		Running = 1 << 1,
		Paused = 1 << 2,
		Stopping = 1 << 3,
		LoadRequested = 1 << 4
	};

protected:
	virtual ~Engine();

protected:
	virtual void TickSpecific() = 0;
	virtual int InitSpecific() = 0;

	[[nodiscard]] virtual int64_t GetTimeSpecific() const = 0;
	[[nodiscard]] virtual double GetTimeIntervalsInSec(int64_t start, int64_t stop) const = 0;

	virtual std::unique_ptr<TDevice> CreateDeviceSpecific(const CDS_MODE cdsMode) = 0;
	virtual void BeginRenderSceneSpecific() = 0;
	virtual void EndRenderSceneSpecific() = 0;

private:
	int InitAnimation();
	bool RenderScene(const float DeltaTime);
	void UnloadLevel();
	void Cleanup();
	int LoadLevel(bool MainMenu);
	bool InitMainMenu();
	bool InitLevel();

	void Tick();
	bool TickScene(float DeltaTime) const;

private:
	/// <summary>
	/// The current state of the engine, can have multiple values at a time.
	/// The values are defined via \ref EngineState flags.
	/// </summary>
	uint32_t CurrentState = States::Initialization;

	/// <summary>
	/// Set a specific state flag to be true for the current state.
	/// </summary>
	void SetState(const States& State) noexcept { CurrentState |= State; }
	/// <summary>
	/// Remove a specific state flag from the current state.
	/// </summary>
	void UnsetState(const States& State) noexcept { CurrentState &= ~State; }

	/// <summary>
	/// Test if a given state is set in the current state.
	/// </summary>
	bool IsStateSet(const States& State) const noexcept { return (CurrentState & State) == State; }

public:
	bool GodMod = false;

	[[nodiscard]] bool IsStopping() const noexcept { return IsStateSet(States::Stopping); }
	[[nodiscard]] bool IsPaused() const noexcept { return IsStateSet(States::Paused); }

	void Stop() noexcept { CurrentState |= States::Stopping; }
	void Pause() noexcept { CurrentState |= States::Paused; }
	void UnPause() noexcept { CurrentState &= ~States::Paused; }
	void RequestLoadLevel() noexcept { CurrentState |= States::LoadRequested; }

public:
	DirectX::XMMATRIX MatView{};
	DirectX::XMMATRIX MatProj{};
	DirectX::XMMATRIX MatViewProj{};

	std::unique_ptr<TDevice> Device;

	ResourcesManager ResourcesManager;
	EffectManager EffectManager;

protected:
	// Physic variable
	const float PhysicDeltaStep = 1.0f / 60.0f;
	float PhysicAccumulator = 0;
	int64_t CurrentPhysicTime = 0;

	// Rendering variable
	int64_t NextTimeStep = 0;
	int64_t PreviousTimeStep = 0;

	Scene* CurrentScene = nullptr;

private:
	static ULONG_PTR GDIToken;
};

template <class T, class TDevice>
ULONG_PTR Engine<T, TDevice>::GDIToken = 0;

template <class T, class TDevice>
void Engine<T, TDevice>::Run()
{
	// We have passed the init phase so we are now running
	UnsetState(States::Initialization);
	SetState(States::Running);

	while (!IsStopping()) {
		// Platfom specific stuff, ignore pause state or the app might freeze
		// -> keep treating windows events, keyboard events...
		TickSpecific();

		Tick();
	}
}

template <class T, class TDevice>
int Engine<T, TDevice>::Init()
{
	SetState(States::Initialization);

	const Gdiplus::GdiplusStartupInput  startupInput(nullptr, TRUE, TRUE);
	Gdiplus::GdiplusStartupOutput startupOutput{};

	GdiplusStartup(&GDIToken, &startupInput, &startupOutput);

	InitSpecific();

	Device = CreateDeviceSpecific(CDS_MODE::CDS_FENETRE);

	LoadLevel(true);

	return 0;
}

template <class T, class TDevice>
Engine<T, TDevice>::~Engine()
{
	Cleanup();
}

template <class T, class TDevice>
void Engine<T, TDevice>::Tick()
{
	const int64_t CurrentTimeStep = GetTimeSpecific();
	const double DeltaTime = GetTimeIntervalsInSec(PreviousTimeStep, CurrentTimeStep);

	const double PhysicElapsedTime = GetTimeIntervalsInSec(CurrentPhysicTime, CurrentTimeStep);
	CurrentPhysicTime = CurrentTimeStep;

	// Update physic state
	PhysicAccumulator += static_cast<float>(PhysicElapsedTime);
	while (PhysicAccumulator >= PhysicDeltaStep) {
		CurrentScene->FixedTick(PhysicDeltaStep);
		if (!IsPaused()) PhysicManager::GetInstance().Step(PhysicDeltaStep);
		PhysicAccumulator -= PhysicDeltaStep;
	}

	if (IsStateSet(States::LoadRequested))
	{
		UnloadLevel();
		LoadLevel(false);
		UnsetState(States::LoadRequested);
	}

	if (DeltaTime > MSPerFrame)
	{
		InputManager::GetInstance().Tick();

		TickScene(static_cast<float>(DeltaTime));

		// Optimized rendering
		Device->Present();
		RenderScene(static_cast<float>(DeltaTime));
		
		PreviousTimeStep = CurrentTimeStep;
	}
}

template <class T, class TDevice>
bool Engine<T, TDevice>::TickScene(float DeltaTime) const
{
	if (!IsPaused())
	{
		// Make all component Tick
		CurrentScene->Tick(DeltaTime);
	}

	CurrentScene->UITick(DeltaTime);

	// Update the scene to reflect components changes
	CurrentScene->Update();

	return true;
}

template <class T, class TDevice>
int Engine<T, TDevice>::InitAnimation()
{
	NextTimeStep = GetTimeSpecific();
	PreviousTimeStep = NextTimeStep;
	CurrentPhysicTime = NextTimeStep;

	// Render a first frame
	RenderScene(0.f);

	return true;
}

template <class T, class TDevice>
bool Engine<T, TDevice>::RenderScene(const float DeltaTime)
{
	BeginRenderSceneSpecific();

	// Start applying effect if any activated
	if (EffectManager.HasEffectActivated()) {
		auto effect = EffectManager.GetActivatedEffect();
		effect->DebutPostEffect();
	}

	// Tick the skybox first
	CurrentScene->SkyBox->LateTick(DeltaTime);

	// Get actors in vision range
	const auto Actors = CurrentScene->GetVisibleActors();

	for (const auto& Actor : Actors) {
		Actor->LateTick(DeltaTime);
	}

	for (const auto& Actor : Actors) {
		Actor->SpriteTick(DeltaTime);
	}

	// Finish applying effect if present
	if (EffectManager.HasEffectActivated()) {
		auto effect = EffectManager.GetActivatedEffect();
		effect->FinPostEffect();
		effect->Draw();
	}

	EndRenderSceneSpecific();
	return true;
}

template <class T, class TDevice>
int Engine<T, TDevice>::LoadLevel(bool MainMenu)
{
	using namespace DirectX;

	CurrentScene = new Scene{};

	PhysicManager::GetInstance().Init();
	PhysicManager::GetInstance().InitScene(CurrentScene->PhysxScene);

	if (MainMenu)
	{
		if (!InitMainMenu()) {
			return 1;
		}
	}
	else
	{
		if (!InitLevel()) {
			return 1;
		}
	}

	MatView = XMMatrixLookAtLH(
		XMVectorSet(0.0f, 3.0f, -5.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

	const float FOV = XM_PI / 2; 	// 45 degr�s
	const float AspectRatio = static_cast<float>(Device->ScreenWidth) / static_cast<float>(Device->ScreenHeight);
	const float NearPlane = 1.0;
	const float FarPlane = 300.0;

	MatProj = XMMatrixPerspectiveFovLH(
		FOV,
		AspectRatio,
		NearPlane,
		FarPlane);
	
	MatViewProj = MatView * MatProj;

	CurrentScene->Init();

	InitAnimation();

	return 0;
}

template <class T, class TDevice>
void Engine<T, TDevice>::UnloadLevel()
{
	delete CurrentScene;

	ResourcesManager.Cleanup();
	PhysicManager::GetInstance().Cleanup();
}

template <class T, class TDevice>
bool Engine<T, TDevice>::InitMainMenu()
{
	GameFactory::GetInstance().LoadMainMenu();

	return true;
}

template <class T, class TDevice>
bool Engine<T, TDevice>::InitLevel()
{
	GameFactory::GetInstance().LoadLevel();

	return true;
}

template <class T, class TDevice>
void Engine<T, TDevice>::Cleanup()
{
	UnloadLevel();

	Gdiplus::GdiplusShutdown(GDIToken);
}

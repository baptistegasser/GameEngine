#include "stdafx.h"
#include "Scene.h"

#include "render/MeshRenderer.h"
#include "math/Math.h"

Scene::Scene()
	: PhysxScene{ nullptr }
	, Tree{ ActorTree{BoundingBox{ 5000.f }} }
	, LightManager{ BoundingBox{ 5000.f } }
	, VisionVolume{ BoundingSphere{ 0.f } }
{}

Scene::~Scene() noexcept
{
	SetSkyBox(nullptr);

	for (auto& actor : AlwaysVisibleActors) {
		delete actor;
		actor = nullptr;
	}
}

void Scene::Tick(const float ElapsedTime)
{
	for (auto& Actor : Tree.GetDatas()) {
		Actor->Tick(ElapsedTime);
	}
}

void Scene::FixedTick(const float DeltaTime)
{
	for (auto& Actor : Tree.GetDatas()) {
		Actor->FixedTick(DeltaTime);
	}
	for (auto& Actor : AlwaysVisibleActors) {
		Actor->FixedTick(DeltaTime);
	}
}

void Scene::LateTick(const float ElapsedTime)
{
	for (auto& Actor : Tree.GetDatas()) {
		Actor->LateTick(ElapsedTime);
	}
	for (auto& Actor : AlwaysVisibleActors) {
		Actor->LateTick(ElapsedTime);
	}
}

void Scene::UITick(const float ElapsedTime)
{
	for (auto& Actor : Tree.GetDatas()) {
		Actor->UITick(ElapsedTime);
	}
	for (auto& Actor : AlwaysVisibleActors) {
		Actor->UITick(ElapsedTime);
	}
}

void Scene::Update()
{
	// Update the Octree
	Tree.Update();
}

void Scene::Init() const
{
	for (auto& Actor : Tree.GetDatas()) {
		Actor->Init();
	}
	for (auto& Actor : AlwaysVisibleActors) {
		Actor->Init();
	}
}

void Scene::AddActor(ActorPtr Actor, bool AlwaysVisible)
{
	if (AlwaysVisible) {
		AlwaysVisibleActors.push_back(Actor);
	}
	else {
		Tree.Add(Actor);
	}
}

void Scene::SetSkyBox(ActorPtr Actor)
{
	if (SkyBox) { 
		delete SkyBox;
	}
	SkyBox = Actor;
}

void Scene::SetCurrentCamera(const Camera* NewCamera) noexcept
{
	CurrentCamera = NewCamera;
}

const Camera& Scene::GetCurrentCamera() const noexcept
{
	return *CurrentCamera;
}

const Scene::ActorPtrList Scene::GetVisibleActors() noexcept
{
	if (Tree.GetDatas().empty() || !CurrentCamera) return ActorPtrList{};

	VisionVolume = BoundingSphere{ 100.f, Math::XMVector2PX(CurrentCamera->GetPosition()) };
	auto Actors = Tree.Find(VisionVolume);
	ConcatVisibleActors(Actors);
	return Actors;
}

const LightManager::LightList Scene::GetVisibleLights() noexcept
{
	VisionVolume = BoundingSphere{ 100.f, Math::XMVector2PX(CurrentCamera->GetPosition()) };
	auto Lights = LightManager.GetLights(VisionVolume);
	return Lights;
}

void Scene::ConcatVisibleActors(ActorPtrList& Actors)
{
	for (auto& VisibleActor : AlwaysVisibleActors)
	{
		Actors.push_back(VisibleActor);
	}
}

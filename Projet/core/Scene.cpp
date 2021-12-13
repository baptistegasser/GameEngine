#include "stdafx.h"
#include "Scene.h"

#include "render/MeshRenderer.h"
#include "math/Math.h"

Scene::Scene()
	: PhysxScene{ nullptr }
	, Tree{ BoundingBox{ 5000.f } }
	, VisionVolume{ BoundingSphere{ 0.f } }
{}

void Scene::Tick(const float ElapsedTime)
{
	for (auto& Actor : Tree.GetActors()) {
		Actor->Tick(ElapsedTime);
	}
}

void Scene::FixedTick(const float DeltaTime)
{
	for (auto& Actor : Tree.GetActors()) {
		Actor->FixedTick(DeltaTime);
	}
	for (auto& Actor : AlwaysVisibleActors) {
		Actor->FixedTick(DeltaTime);
	}
}

void Scene::LateTick(const float ElapsedTime)
{
	for (auto& Actor : Tree.GetActors()) {
		Actor->LateTick(ElapsedTime);
	}
	for (auto& Actor : AlwaysVisibleActors) {
		Actor->LateTick(ElapsedTime);
	}
}

void Scene::Update()
{
	// Update the Octree
	Tree.Update();
}

void Scene::Init() const
{
	for (auto& Actor : Tree.GetActors()) {
		Actor->Init();
	}
	for (auto& Actor : AlwaysVisibleActors) {
		Actor->Init();
	}
}

void Scene::AddActor(ActorPtr Actor, bool AlwaysVisible)
{
	if (AlwaysVisible) {
		AlwaysVisibleActors.push_back(std::move(Actor));
	}
	else {
		Tree.Add(std::move(Actor));
	}
}

void Scene::AddSkyBox(ActorPtr Actor)
{
	SkyBox = std::move(Actor);
}

void Scene::SetCurrentCamera(const Camera* NewCamera) noexcept
{
	CurrentCamera = NewCamera;
}

const Camera& Scene::GetCurrentCamera() const noexcept
{
	return *CurrentCamera;
}

const Octree::ActorPtrList Scene::GetVisibleActors() noexcept
{
	VisionVolume = BoundingSphere{ 100.f, Math::XMVector2PX(CurrentCamera->GetPosition()) };
	auto Actors = Tree.Find(VisionVolume);
	ConcatVisibleActors(Actors);
	return Actors;
}

physx::PxRaycastBuffer Scene::Raycast(const Math::Vec3f Origin, const Math::Vec3f Direction, float Distance) const
{
	physx::PxRaycastBuffer Hit;
	PhysxScene->raycast(Origin, Direction, Distance, Hit);
	return Hit;
}

void Scene::ConcatVisibleActors(Octree::ActorPtrList& Actors)
{
	for (auto& VisibleActor : AlwaysVisibleActors)
	{
		Actors.push_back(VisibleActor.get());
	}
}

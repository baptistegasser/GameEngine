#include "stdafx.h"
#include "Scene.h"

#include "render/MeshRenderer.h"
#include "math/Math.h"

Scene::Scene()
	: PhysxScene{ nullptr }
	, Tree{ BoundingBox{ 10000.f } }
	, VisionVolume{ BoundingSphere{ 0.f } }
{}

void Scene::Tick(const float ElapsedTime)
{
	for (auto& Actor : GetActors()) {
		Actor->Tick(ElapsedTime);
	}
}

void Scene::FixedTick(const float DeltaTime)
{
	for (auto& Actor : GetActors()) {
		Actor->FixedTick(DeltaTime);
	}
}

void Scene::LateTick(const float ElapsedTime)
{
	for (auto& Actor : GetActors()) {
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
}

void Scene::AddActor(ActorPtr Actor)
{
	Tree.Add(std::move(Actor));
}

const Octree::ActorList& Scene::GetActors()
{
	return Tree.GetActors();
}

void Scene::SetCurrentCamera(const Camera* NewCamera) noexcept
{
	CurrentCamera = NewCamera;
}

const Octree::ActorPtrList Scene::GetVisibleActors() noexcept
{
	VisionVolume = BoundingSphere{ 1000.f, Math::XMVector2PX(CurrentCamera->GetPosition()) };
	return Tree.Find(VisionVolume);
}

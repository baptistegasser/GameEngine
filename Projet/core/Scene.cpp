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
	for (auto& actor : GetActors()) {
		auto Components = actor->GetNotFlaggedComponents(Pitbull::Component::RENDER_COMPONENT);

		for (const auto& Comp : Components) {
			Comp->Tick(ElapsedTime);
		}
	}
}

void Scene::FixedTick(const float DeltaTime)
{
	for (auto& actor : GetActors()) {
		actor->FixedTick(DeltaTime);
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
	VisionVolume = BoundingSphere{ 10.f, Math::XMVector2PX(CurrentCamera->GetPosition()) };
	return Tree.Find(VisionVolume);
}

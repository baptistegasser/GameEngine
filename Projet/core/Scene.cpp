#include "stdafx.h"
#include "Scene.h"

#include "render/MeshRenderer.h"

Scene::Scene()
	: PhysxScene{ nullptr }
	, Tree{ BoundingBox{ 10000.f } }
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

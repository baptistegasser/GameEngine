#include "stdafx.h"
#include "Scene.h"

#include "render/MeshRenderer.h"

Scene::Scene()
	: PhysxScene{ nullptr }
	, Tree{ BoundingBox{ 10000.f } }
{}

void Scene::Update(const float Elapsed)
{
	// Update non render actor
	for (auto& actor : GetActors())
	{
		auto& Components = actor->GetComponents();

		for (const auto& Comp : Components)
		{
			if (dynamic_cast<MeshRenderer*>(Comp) == nullptr)
			{
				Comp->Tick(Elapsed);
			}
		}
	}

	// Update Tree
	Tree.Update();
}

void Scene::Init()
{
	for (auto& Actor : Tree.GetActors())
	{
		Actor->Init();
	}
}

void Scene::Tick()
{
	for (auto& Actor : Tree.GetActors())
	{
		Actor->Tick(/* TODO deltatime */);
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

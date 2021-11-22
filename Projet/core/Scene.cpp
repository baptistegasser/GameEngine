#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
	: PhysxScene{ nullptr }
	, Tree{ BoundingBox{ 1000.f } }
{}

void Scene::Init()
{
	for (auto& Actor : Actors)
	{
		Actor->Init();
	}
}

void Scene::Tick()
{
	for (auto& Actor : Actors)
	{
		Actor->Tick(/* TODO deltatime */);
	}
}

void Scene::AddActor(ActorPtr& Actor)
{
	Actors.push_back(Actor);
}

std::vector<Scene::ActorPtr>& Scene::GetActors()
{
	return Actors;
}

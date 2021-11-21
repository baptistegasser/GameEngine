#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
	: PhysxScene{ nullptr }
{}

void Scene::Init()
{
	for (auto& Actor : Actors)
	{
		Actor.Init();
	}
}

void Scene::Tick()
{
	for (auto& Actor : Actors)
	{
		Actor.Tick(/* TODO deltatime */);
	}
}

void Scene::AddActor(Pitbull::Actor& Actor)
{
	Actors.push_back(std::move(Actor));
}

std::vector<Pitbull::Actor>& Scene::GetActors()
{
	return Actors;
}

#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
	: PhysxScene{ nullptr }
	, SceneContactHandler{ std::make_unique<ContactHandler>() }
{}

void Scene::Tick()
{
	for (const auto& Actor : Actors)
	{
		Actor->Tick(/* TODO deltatime */);
	}
}

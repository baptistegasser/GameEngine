#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
	: SceneContactHandler{ std::make_unique<ContactHandler>() }
{}

void Scene::Tick()
{
	for (auto& Actor : Actors)
	{
		Actor->Tick(/* TODO deltatime */);
	}
}

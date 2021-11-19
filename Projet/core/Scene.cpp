#include "stdafx.h"
#include "Scene.h"

Scene::Scene()
	: PhysxScene{ nullptr }
	, SceneContactHandler{ nullptr }
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

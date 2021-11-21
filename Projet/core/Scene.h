#pragma once

#include "core/Actor.h"
#include "physic/ContactHandler.h"

#include "PxPhysicsAPI.h"
#include <vector>

class Scene {
public:
	Scene();
	~Scene() = default;

	void Init();
	void Tick();

	void AddActor(Pitbull::Actor& Actor);
	std::vector<Pitbull::Actor>& GetActors();

	physx::PxScene* PhysxScene;
	ContactHandler SceneContactHandler;

private:
	std::vector<Pitbull::Actor> Actors;
};

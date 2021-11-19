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

	physx::PxScene* PhysxScene;
	ContactHandler* SceneContactHandler;
	std::vector<Pitbull::Actor> Actors;
};

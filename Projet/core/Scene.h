#pragma once

#include "core/Actor.h"
#include "physic/ContactHandler.h"

#include "PxPhysicsAPI.h"
#include <vector>
#include <memory>

class Scene {
public:
	Scene();

	void Tick();

	physx::PxScene* PhysxScene;
	std::unique_ptr<ContactHandler> SceneContactHandler;
	std::vector<std::unique_ptr<Pitbull::Actor>> Actors;
};

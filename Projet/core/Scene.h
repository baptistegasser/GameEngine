#pragma once

#include "core/Actor.h"
#include "physic/ContactHandler.h"

#include "PxPhysicsAPI.h"
#include <vector>
#include <memory>

class Scene {
public:
	using ActorPtr = std::unique_ptr<Pitbull::Actor>;

	Scene();
	~Scene() = default;

	void Init();
	void Tick();

	void AddActor(ActorPtr& Actor);
	std::vector<ActorPtr>& GetActors();

	physx::PxScene* PhysxScene;
	ContactHandler SceneContactHandler;

private:
	std::vector<ActorPtr> Actors;
};

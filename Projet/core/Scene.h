#pragma once

#include "Octree.h"

#include "PxPhysicsAPI.h"
#include <memory>

class Scene {
public:
	using ActorPtr = Octree::ActorType;

	Scene();
	~Scene() = default;

	void Update(const float Elapsed);
	void Init();
	void Tick();

	void AddActor(ActorPtr Actor);
	const Octree::ActorList& GetActors();

	physx::PxScene* PhysxScene;

	BoundingVolume* VisionVolume;

private:
	Octree Tree;
};

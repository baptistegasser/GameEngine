#pragma once

#include "Octree.h"
#include "render/LightConfig.h"

#include "PxPhysicsAPI.h"
#include <memory>

class Scene {
public:
	using ActorPtr = Octree::ActorType;

	Scene();
	~Scene() = default;

	/// <summary>
	/// Init all actors in the scene.
	/// </summary>
	void Init() const;

	/// <summary>
	/// Call all components Tick() method, except those axed toward rendering.
	/// </summary>
	void Tick(const float ElapsedTime);

	/// <summary>
	/// Call all components FixedTick() method.
	/// </summary>
	void FixedTick(const float DeltaTime);

	/// <summary>
	/// Update the scene state.
	/// </summary>
	void Update();

	void AddActor(ActorPtr Actor);
	const Octree::ActorList& GetActors();

	physx::PxScene* PhysxScene;

	BoundingVolume* VisionVolume;
	LightConfig LightConfig;

//private:
	Octree Tree;
};

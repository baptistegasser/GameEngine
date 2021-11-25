#pragma once

#include "Octree.h"
#include "render/Camera.h"

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

	/// <summary>
	/// Update the camera used to get visible actors.
	/// </summary>
	void SetCurrentCamera(const Camera* NewCamera) noexcept;

	/// <summary>
	/// Get all actors in current vision range.
	/// </summary>
	const Octree::ActorPtrList GetVisibleActors() noexcept;

	physx::PxScene* PhysxScene;

private:
	const Camera* CurrentCamera;
	BoundingVolume VisionVolume;
	Octree Tree;
};

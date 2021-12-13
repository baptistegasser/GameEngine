#pragma once

#include "math/Octree.h"
#include "render/Camera.h"
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
	/// Call all actors Tick() method.
	/// </summary>
	void Tick(const float ElapsedTime);

	/// <summary>
	/// Call all actors FixedTick() method.
	/// </summary>
	void FixedTick(const float DeltaTime);

	/// <summary>
	/// Call all actors LateTick() method.
	/// </summary>
	void LateTick(const float ElapsedTime);

	/// <summary>
	/// Update the scene state.
	/// </summary>
	void Update();

	void AddActor(ActorPtr Actor, bool AlwaysVisible = false);

	void AddSkyBox(ActorPtr Actor);

	/// <summary>
	/// Update the camera used to get visible actors.
	/// </summary>
	void SetCurrentCamera(const Camera* NewCamera) noexcept;
	const Camera& GetCurrentCamera() const noexcept;

	/// <summary>
	/// Get all actors in current vision range.
	/// </summary>
	const Octree::ActorPtrList GetVisibleActors() noexcept;

	physx::PxRaycastBuffer Raycast(const Math::Vec3f Origin, const Math::Vec3f Direction, float Distance) const;

	physx::PxScene* PhysxScene;

	/// <summary>
	/// The lights of the scene
	/// </summary>
	LightConfig LightConfig;

	/// <summary>
	/// The skybox of the scene
	/// </summary>
	Octree::ActorType SkyBox;
private:
	const Camera* CurrentCamera;
	BoundingVolume VisionVolume;
	Octree Tree;
	Octree::ActorList AlwaysVisibleActors;


	void ConcatVisibleActors(Octree::ActorPtrList& Actors);
};

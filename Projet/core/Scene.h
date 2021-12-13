#pragma once

#include "Actor.h"
#include "math/Octree.h"
#include "render/Camera.h"
#include "render/LightManager.h"

#include "PxPhysicsAPI.h"

#include <memory>

struct ActorToPos
{
	Math::Vec3f operator()(const std::unique_ptr<Pitbull::Actor>& Actor) const noexcept
	{
		return Actor->Transform.Position;
	}
};

class Scene {
public:
	using ActorTree = Octree<Pitbull::Actor, ActorToPos>;
	using ActorPtr = ActorTree::DataType;
	using ActorPtrList = ActorTree::DataPtrList;

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

	/// <summary>
	/// Update the camera used to get visible actors.
	/// </summary>
	void SetCurrentCamera(const Camera* NewCamera) noexcept;
	const Camera& GetCurrentCamera() const noexcept;

	/// <summary>
	/// Get all actors in current vision range.
	/// </summary>
	const ActorPtrList GetVisibleActors() noexcept;

	/// <summary>
	/// Get all actors in current vision range.
	/// </summary>
	const LightManager::LightList GetVisibleLights() noexcept;

	physx::PxRaycastBuffer Raycast(const Math::Vec3f Origin, const Math::Vec3f Direction, float Distance) const;

	physx::PxScene* PhysxScene;
	/// <summary>
	/// Manager that keep track of lights in the scene.
	/// </summary>
	LightManager LightManager;

private:
	const Camera* CurrentCamera;
	BoundingVolume VisionVolume;
	ActorTree* Tree;
	ActorTree::DataList AlwaysVisibleActors;

	void ConcatVisibleActors(ActorPtrList& Actors);
};

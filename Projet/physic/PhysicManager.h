#pragma once

#ifndef PHYSX_SNIPPET_PVD_H
#define PHYSX_SNIPPET_PVD_H
//Set this to the IP address of the system running the PhysX Visual Debugger that you want to connect to.
#define PVD_HOST "127.0.0.1"
#endif

#include "util/Singleton.h"
#include "core/Scene.h"
#include "ContactHandler.h"
#include "RigidBody.h"

#include "PxPhysicsAPI.h"

#include <memory>

class PhysicManager : public Singleton<PhysicManager>
{
	friend struct Singleton<PhysicManager>;

public:
	void Init();
	void InitScene(physx::PxScene* Scene);
	void Step(const float& ElapsedTime);
	void Cleanup();

	void RegisterRigidBody(RigidBody* RigidBody);
	ContactHandler& GetContactHandler() noexcept;

	physx::PxPhysics* Physics = nullptr;
	physx::PxCooking* Cooking = nullptr;
	physx::PxScene* CurrentScene;

private:
	PhysicManager() = default;
	~PhysicManager() = default;
	static physx::PxDefaultAllocator Allocator;
	static physx::PxDefaultErrorCallback ErrorCallback;

	physx::PxFoundation* Foundation = nullptr;

	physx::PxPvd* Pvd = nullptr;
	physx::PxDefaultCpuDispatcher* Dispatcher = nullptr;

	ContactHandler ContactHandler;
	/// <summary>
	/// List of all RigidBody created during runtime.
	/// </summary>
	std::vector<RigidBody*> RigidBodies;
};

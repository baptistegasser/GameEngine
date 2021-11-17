#pragma once

#ifndef PHYSX_SNIPPET_PVD_H
#define PHYSX_SNIPPET_PVD_H
//Set this to the IP address of the system running the PhysX Visual Debugger that you want to connect to.
#define PVD_HOST "127.0.0.1"
#endif

#include "util/Singleton.h"
#include "util/Util.h" // PX_RELEASE
#include "core/Scene.h"

#include "PxPhysicsAPI.h"

#include <memory>

class PhysicManager : public Singleton<PhysicManager>
{
	friend struct Singleton<PhysicManager>;

public:
	void Init();
	void InitScene(std::shared_ptr<Scene> Scene);
	void Step();
	void Cleanup();

	physx::PxPhysics* Physics = nullptr;
	std::shared_ptr<Scene> CurrentScene;

private:
	PhysicManager() {};
	~PhysicManager() noexcept {};
	physx::PxDefaultAllocator Allocator;
	physx::PxDefaultErrorCallback ErrorCallback;

	physx::PxFoundation* Foundation = nullptr;

	physx::PxPvd* Pvd = nullptr;
	physx::PxDefaultCpuDispatcher* Dispatcher = nullptr;
};

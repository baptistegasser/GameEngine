#pragma once

#ifndef PHYSX_SNIPPET_PVD_H
#define PHYSX_SNIPPET_PVD_H
//Set this to the IP address of the system running the PhysX Visual Debugger that you want to connect to.
#define PVD_HOST "127.0.0.1"
#endif

#define PX_RELEASE(x) if(x) { x->release(); x = NULL; }

#include "util/Singleton.h"
#include "core/Scene.h"

#include "PxPhysicsAPI.h"

#include <memory>

class PhysicManager : public Singleton<PhysicManager>
{
public:
	void Init();
	void InitScene(std::shared_ptr<Scene> Scene);
	void Step();
	void Cleanup();

private:
	std::shared_ptr<Scene> CurrentScene;

	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;

	physx::PxPhysics* gPhysics = nullptr;
	physx::PxFoundation* gFoundation = nullptr;

	physx::PxPvd* gPvd = nullptr;
	physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
};

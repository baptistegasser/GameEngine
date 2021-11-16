#include "stdafx.h"
#include "PhysicManager.h"

#include "ContactFilter.h"

using namespace physx;

void PhysicManager::Init()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gDispatcher = PxDefaultCpuDispatcherCreate(2);
}

void PhysicManager::InitScene(std::shared_ptr<Scene> Scene)
{
	CurrentScene = Scene;

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;

	// Change physic handling
	sceneDesc.filterShader = ContactFilter::filterShader;
	sceneDesc.contactModifyCallback = CurrentScene->SceneContactHandler.get();
	sceneDesc.simulationEventCallback = CurrentScene->SceneContactHandler.get();

	CurrentScene->PhysxScene = std::unique_ptr<PxScene>(gPhysics->createScene(sceneDesc));

	// Finally init the scene for the Physx visual debugger
	PxPvdSceneClient* pvdClient = CurrentScene->PhysxScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

void PhysicManager::Step()
{
	CurrentScene->PhysxScene->simulate(1.0f / 60.0f);
	CurrentScene->PhysxScene->fetchResults(true);
}

void PhysicManager::Cleanup()
{
	PX_RELEASE(CurrentScene->PhysxScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		PX_RELEASE(gPvd);
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);
}


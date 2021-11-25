#include "stdafx.h"
#include "PhysicManager.h"

#include "util/Util.h" // PX_RELEASE
#include "ContactFilter.h"

using namespace physx;

PxDefaultAllocator PhysicManager::Allocator;
PxDefaultErrorCallback PhysicManager::ErrorCallback;

void PhysicManager::Init()
{
	Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, Allocator, ErrorCallback);

	Pvd = PxCreatePvd(*Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation, PxTolerancesScale(), true, Pvd);

	Dispatcher = PxDefaultCpuDispatcherCreate(2);
}

void PhysicManager::InitScene(std::shared_ptr<Scene> Scene)
{
	CurrentScene = Scene;

	PxSceneDesc sceneDesc(Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -0.1f, 0.0f);
	Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = Dispatcher;

	// Change physic handling
	sceneDesc.filterShader = ContactFilter::filterShader;
	sceneDesc.contactModifyCallback = &ContactHandler;
	sceneDesc.simulationEventCallback = &ContactHandler;

	CurrentScene->PhysxScene = Physics->createScene(sceneDesc);

	// Finally init the scene for the Physx visual debugger
	PxPvdSceneClient* pvdClient = CurrentScene->PhysxScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

void PhysicManager::Step(const float& ElapsedTime)
{
	const auto CallPreTick = [](const RigidBody* RB) { RB->PreFixedTick(); };
	std::for_each(RigidBodies.begin(), RigidBodies.end(), CallPreTick);

	CurrentScene->PhysxScene->simulate(ElapsedTime);
	CurrentScene->PhysxScene->fetchResults(true);
}

void PhysicManager::Cleanup()
{
	PX_RELEASE(CurrentScene->PhysxScene);
	PX_RELEASE(Dispatcher);
	PX_RELEASE(Physics);
	if (Pvd)
	{
		PxPvdTransport* transport = Pvd->getTransport();
		PX_RELEASE(Pvd);
		PX_RELEASE(transport);
	}
	PX_RELEASE(Foundation);
}

void PhysicManager::RegisterRigidBody(const RigidBody* RigidBody)
{
	CurrentScene->PhysxScene->addActor(*RigidBody->RigidActor);
	RigidBodies.push_back(RigidBody);
}

ContactHandler& PhysicManager::GetContactHandler() noexcept
{
	return ContactHandler;
}


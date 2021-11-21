#include "stdafx.h"
#include "RigidBody.h"

#include "Collider.h"
#include "util/Util.h"

using namespace physx;

RigidBody::RigidBody(Pitbull::Actor* Parent, bool IsStatic, bool DisableGravity, float Mass, PxVec3 Velocity)
	: Component{ Parent }
	, IsStatic{ IsStatic }
	, DisableGravity{ DisableGravity }
	, Mass{ Mass }
	, Velocity{ Velocity }
{}

RigidBody::~RigidBody()
{
	std::cout << "debug";
	PX_RELEASE(RigidActor);
}

void RigidBody::Init()
{
	const auto Physics = PhysicManager::GetInstance().Physics;

	if (IsStatic) {
		RigidActor = Physics->createRigidStatic(ParentActor->Transform);
	}
	else {
		const auto DynamicActor = Physics->createRigidDynamic(ParentActor->Transform);
		DynamicActor->setMass(Mass);
		DynamicActor->setLinearVelocity(Velocity);
		DynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, DisableGravity);
		RigidActor = DynamicActor;
	}

	const auto Colliders = ParentActor->GetComponents<Collider>();
	for( const auto Collider : Colliders )
	{
		auto TMPMat = Collider->GetPxMaterial();
		auto TMPGeo = Collider->GetPxGeometry();

		PxRigidActorExt::createExclusiveShape(*RigidActor, *TMPGeo, *TMPMat);
	}

	PhysicManager::GetInstance().CurrentScene->PhysxScene->addActor(*RigidActor);
}

void RigidBody::Tick(const float& ElapsedTime)
{
	// Update parent pos with self, simulated pos
	ParentActor->Transform = RigidActor->getGlobalPose();
}

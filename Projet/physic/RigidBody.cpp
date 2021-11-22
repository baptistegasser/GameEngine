#include "stdafx.h"
#include "RigidBody.h"

#include "Collider.h"
#include "util/Util.h"
#include "PhysicManager.h"

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
	PX_RELEASE(RigidActor);
}

void RigidBody::Init()
{
	auto& PhysicManager = PhysicManager::GetInstance();
	const auto Physics = PhysicManager.Physics;

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
	for( const auto Collider : Colliders) {
		const PxShape* Shape = PxRigidActorExt::createExclusiveShape(*RigidActor, *Collider->GetPxGeometry(), *Collider->GetPxMaterial());
		PhysicManager.GetContactHandler().RegisterCollider(Shape, Collider);
	}

	PhysicManager::GetInstance().CurrentScene->PhysxScene->addActor(*RigidActor);
}

void RigidBody::Tick(const float& ElapsedTime)
{
	// Update parent pos with self, simulated pos
	ParentActor->Transform = RigidActor->getGlobalPose();
}

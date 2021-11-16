#include "stdafx.h"
#include "RigidBody.h"

#include "Collider.h"

void RigidBody::Init()
{
	using namespace physx;

	const auto Physics = PhysicManager::GetInstance().Physics;

	if (IsStatic) {
		RigidActor = Physics->createRigidStatic(ParentActor->Transform);
	}
	else {
		const auto DynamicActor = Physics->createRigidDynamic(ParentActor->Transform);
		DynamicActor->setMass(Mass);
		DynamicActor->setLinearVelocity(Velocity);
		DynamicActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, UseGravity);
		RigidActor = DynamicActor;
	}

	const auto Colliders = ParentActor->GetComponents<Collider>();
	for( const auto Collider : Colliders )
	{
		RigidActor->attachShape(*Physics->createShape(*Collider->GetPxGeometry(), *Collider->GetPxMaterial()));
	}
}

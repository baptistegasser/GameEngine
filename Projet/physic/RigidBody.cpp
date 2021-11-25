#include "stdafx.h"
#include "RigidBody.h"

#include "Collider.h"
#include "util/Util.h"
#include "PhysicManager.h"
#include "../math/Math.h"

#include <cassert>

#ifdef _DEBUG
#define ASSERT_DYNAMIC assert("This method must be called on dynamic actor only" && !IsStatic);
#endif

using namespace physx;

RigidBody::RigidBody(Pitbull::Actor* Parent, bool IsStatic)
	: Component{ Parent }
	, IsStatic{ IsStatic }
	, RigidActor{ nullptr }
{
	TypeFlags |= PHYSIC_COMPONENT;
}

RigidBody::~RigidBody()
{
	PX_RELEASE(RigidActor);
}

void RigidBody::Init()
{
	auto& PhysicManager = PhysicManager::GetInstance();
	const auto Physics = PhysicManager.Physics;

	// Create appropriate physx actor
	if (IsStatic) {
		RigidActor = Physics->createRigidStatic(ParentActor->Transform);
	}
	else {
		RigidActor = Physics->createRigidDynamic(ParentActor->Transform);
	}

	// Create and add shapes for collisions
	const auto Colliders = ParentActor->GetComponents<Collider>();
	for( const auto Collider : Colliders) {
		PxShape* Shape = Physics->createShape(*Collider->GetPxGeometry(), *Collider->GetPxMaterial());
		RigidActor->attachShape(*Shape);
		PhysicManager.GetContactHandler().RegisterCollider(Shape, Collider);
	}

	// Notify the big boss that we exist
	PhysicManager.RegisterRigidBody(this);
}

void RigidBody::PreFixedTick() const
{
	// Update the transform with the actor's own to match change made by other components
	RigidActor->setGlobalPose(ParentActor->Transform);
}

void RigidBody::FixedTick(const float& DeltaTime)
{
	// Update the actor transform with the simulated one
	ParentActor->Transform = RigidActor->getGlobalPose();
}

physx::PxRigidDynamic* RigidBody::GetAsDynamic() const noexcept
{
	return static_cast<PxRigidDynamic*>(RigidActor); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
}

void RigidBody::AddForce(const physx::PxVec3& Force, const ForceMode& ForceMode) const
{
	ASSERT_DYNAMIC
	GetAsDynamic()->addForce(Force, PhysxForce(ForceMode));
}

void RigidBody::AddTorque(const physx::PxVec3& Torque, const ForceMode& ForceMode) const
{
	ASSERT_DYNAMIC
	GetAsDynamic()->addTorque(Torque, PhysxForce(ForceMode));
}

void RigidBody::SetVelocity(const physx::PxVec3& Velocity) const
{
	ASSERT_DYNAMIC
	GetAsDynamic()->setLinearVelocity(Velocity);
}

void RigidBody::SetMass(const float Mass) const
{
	ASSERT_DYNAMIC
	GetAsDynamic()->setMass(Mass);
}

void RigidBody::SetFollowGravity(const bool FollowGravity) const
{
	RigidActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !FollowGravity);
}

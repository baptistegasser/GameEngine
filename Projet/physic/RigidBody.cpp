#include "stdafx.h"
#include "RigidBody.h"

#include "Collider.h"
#include "util/Util.h"
#include "PhysicManager.h"
#include "../math/Math.h"

#include <cassert>

#ifdef _DEBUG
#define ASSERT_DYNAMIC assert("This method must be called on a dynamic body only" && IsDynamic());
#define ASSERT_KINEMATIC assert("This method must be called on a kinematic body only" && IsKinematic());
#define ASSERT_NOT_KINEMATIC assert("This method can not be called on a kinematic body" && !IsKinematic());
#endif

using namespace physx;

RigidBody::RigidBody(Pitbull::Actor* Parent, RigidActorType ActorType, bool IsTrigger)
	: Component{ Parent }
	, ActorType{ ActorType }
	, RigidActor{ nullptr }
	, IsTrigger{ IsTrigger }
{
	const auto Physics = PhysicManager::GetInstance().Physics;

	// Create appropriate physx actor
	switch (ActorType) {
	case RigidActorType::Static:
		RigidActor = Physics->createRigidStatic(ParentActor->Transform);
		break;
	case RigidActorType::Dynamic:
		RigidActor = Physics->createRigidDynamic(ParentActor->Transform);
		break;
	case RigidActorType::Kinematic:
		RigidActor = Physics->createRigidDynamic(ParentActor->Transform);
		static_cast<PxRigidDynamic*>(RigidActor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		break;
	default: throw "Invalid actor type";
	}
}

void RigidBody::PrePhysicSimulation() noexcept
{
	// Update the transform with the actor's own to match change made by other components
	auto a = ParentActor->Transform.operator physx::PxTransform();
	RigidActor->setGlobalPose(ParentActor->Transform);
	// Change scale only if it changed
	if (PreviousScale != ParentActor->Transform.Scale) {
		UpdateActorShapes();
	}
}

void RigidBody::PostPhysicSimulation() noexcept
{
	// Update the actor transform with the simulated one
	ParentActor->Transform = RigidActor->getGlobalPose();
}

void RigidBody::Init()
{
	// Create and add shapes for collisions
	UpdateActorShapes();

	// Notify the big boss that we exist
	PhysicManager::GetInstance().RegisterRigidBody(this);
}

void RigidBody::UpdateActorShapes() noexcept
{
	auto& PhysicManager = PhysicManager::GetInstance();
	const auto Physics = PhysicManager.Physics;

	// Release current shapes
	const physx::PxU32 ShapesCount = RigidActor->getNbShapes();
	if (ShapesCount) {
		PxShape** Shapes = (PxShape**)malloc(sizeof(PxShape*) * ShapesCount);
		RigidActor->getShapes(Shapes, ShapesCount);
		const auto ClearShape = [this, &PhysicManager](PxShape* Shape) {
			// Unregister associated collider
			PhysicManager.GetContactHandler().ClearRegistredCollider(Shape);
			RigidActor->detachShape(*Shape);
		};
		std::for_each_n(Shapes, ShapesCount, ClearShape);
		free(Shapes);
	}

	// Create new shapes from colliders
	const auto Colliders = ParentActor->GetComponents<Collider>();
	for (const auto Collider : Colliders) {
		PxShape* Shape = Physics->createShape(*Collider->GetPxGeometry(ParentActor->Transform.Scale), *Collider->GetPxMaterial(), true);

		if (IsTrigger)
		{
			Shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			Shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}

		// Create Shape transform based on the actor's transform and the collider offset
		auto LocalTransform = ParentActor->Transform;
		// Default rotation to not duplicate parent's rot
		LocalTransform.Rotation = Math::Quaternion();
		LocalTransform.Position = Collider->Offset;
		Shape->setLocalPose(LocalTransform);

		RigidActor->attachShape(*Shape);
		PhysicManager.GetContactHandler().RegisterCollider(Shape, Collider);
	}

	// Update the scale as change are now taken in account
	PreviousScale = ParentActor->Transform.Scale;
}

const bool RigidBody::IsStatic() const noexcept
{
	return ActorType == RigidActorType::Static;
}

const bool RigidBody::IsDynamic() const noexcept
{
	return ActorType == RigidActorType::Dynamic || ActorType == RigidActorType::Kinematic;
}

const bool RigidBody::IsKinematic() const noexcept
{
	return ActorType == RigidActorType::Kinematic;
}

physx::PxRigidDynamic* RigidBody::GetAsDynamic() const noexcept
{
	return static_cast<PxRigidDynamic*>(RigidActor); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
}

void RigidBody::AddForce(const Math::Vec3f& Force, const ForceMode& ForceMode) const
{
	ASSERT_DYNAMIC ASSERT_NOT_KINEMATIC
	GetAsDynamic()->addForce(Force, PhysxForce(ForceMode));
}

void RigidBody::AddTorque(const Math::Vec3f& Torque, const ForceMode& ForceMode) const
{
	ASSERT_DYNAMIC ASSERT_NOT_KINEMATIC
	GetAsDynamic()->addTorque(Torque, PhysxForce(ForceMode));
}

void RigidBody::ClearForce() const
{
	ASSERT_DYNAMIC ASSERT_NOT_KINEMATIC
	GetAsDynamic()->clearForce();
}

void RigidBody::ClearTorque() const
{
	ASSERT_DYNAMIC ASSERT_NOT_KINEMATIC
	GetAsDynamic()->clearTorque();
}

void RigidBody::SetVelocity(const Math::Vec3f& Velocity) const
{
	ASSERT_DYNAMIC ASSERT_NOT_KINEMATIC
	GetAsDynamic()->setLinearVelocity(Velocity);
}
void RigidBody::ClearVelocity() const
{
	ASSERT_DYNAMIC ASSERT_NOT_KINEMATIC
	GetAsDynamic()->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
}

void RigidBody::ClearAngularVelocity() const
{
	ASSERT_DYNAMIC ASSERT_NOT_KINEMATIC
	GetAsDynamic()->setAngularVelocity(PxVec3(0.0f, 0.0f, 0.0f));
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

void RigidBody::setKinematicTarget(const Math::Transform& Target) const
{
	ASSERT_KINEMATIC;
	GetAsDynamic()->setKinematicTarget(Target);
}

const Math::Vec3f RigidBody::GetVelocity() const
{
	ASSERT_DYNAMIC ASSERT_NOT_KINEMATIC
	return GetAsDynamic()->getLinearVelocity();
}

void RigidBody::setMaxLinearVelocity(float Velocity)
{
	ASSERT_DYNAMIC
	GetAsDynamic()->setMaxLinearVelocity(Velocity);
}

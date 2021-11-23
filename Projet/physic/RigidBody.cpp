#include "stdafx.h"
#include "RigidBody.h"

#include "Collider.h"
#include "util/Util.h"
#include "PhysicManager.h"
#include <render/Player.h>
#include "../math/Math.h"

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
		PxShape* Shape = Physics->createShape(*Collider->GetPxGeometry(), *Collider->GetPxMaterial());
		RigidActor->attachShape(*Shape);
		PhysicManager.GetContactHandler().RegisterCollider(Shape, Collider);
	}

	PhysicManager::GetInstance().CurrentScene->PhysxScene->addActor(*RigidActor);

	auto player = ParentActor->GetComponent<Player>();
	if (player != nullptr) {
		auto rigid = static_cast<PxRigidDynamic*>(RigidActor);
		//if (rigid != nullptr)
			//rigid->setMaxLinearVelocity(0.05f);
	}
}

void RigidBody::Tick(const float& ElapsedTime)
{
	auto player = ParentActor->GetComponent<Player>();
	if (player != nullptr) {
		//RigidActor->setGlobalPose(PxTransform(RigidActor->getGlobalPose().p, PxQuat(player->AngleRotation, PxVec3(0, 1, 0))));
		auto rigid = static_cast<PxRigidDynamic*>(RigidActor);
		if (rigid != nullptr) {
			rigid->setLinearVelocity(Math::XMVector2PX( player->Velocity));

		}
				

			
	}
	
	// Update parent pos with self, simulated pos
	ParentActor->Transform = RigidActor->getGlobalPose();
}

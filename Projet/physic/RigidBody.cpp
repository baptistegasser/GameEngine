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
}

void RigidBody::Tick(const float& ElapsedTime)
{
	// Deplacements of the player
	auto player = ParentActor->GetComponent<Player>();
	if (player != nullptr) {
		auto rigid = static_cast<PxRigidDynamic*>(RigidActor);
		if (rigid != nullptr) {					
			rigid->addForce(Math::XMVector2PX(player->Direction) * 0.01f, physx::PxForceMode::Enum::eIMPULSE);
			if (player->Forward) {
				rigid->addForce(Math::XMVector2PX(player->Direction) * player->Speed, physx::PxForceMode::Enum::eIMPULSE);
			}
			if (player->Backward) {
				rigid->addForce(- Math::XMVector2PX(player->Direction) * player->Speed, physx::PxForceMode::Enum::eIMPULSE);
			}
			if (player->Left) {
				rigid->addForce(Math::XMVector2PX(player->RelativeZ) * player->Speed, physx::PxForceMode::Enum::eIMPULSE);
			}
			if (player->Right) {
				rigid->addForce(-Math::XMVector2PX(player->RelativeZ) * player->Speed, physx::PxForceMode::Enum::eIMPULSE);
			}
			if (player->Jump) {
				rigid->addForce(PxVec3(0.0f, 1.0f, 0.0f) * player->JumpSpeed, physx::PxForceMode::Enum::eIMPULSE);
			}
		}
	}
	
	// Update parent pos with self, simulated pos
	ParentActor->Transform = RigidActor->getGlobalPose();
}

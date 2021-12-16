#include "stdafx.h"
#include "IntelligentEnemy.h"

#include "render/EngineD3D11.h"
#include "math/Math.h"
#include "math/Vec3f.h"

using namespace Math;

IntelligentEnemy::IntelligentEnemy(Pitbull::Actor* Parent, Math::Transform* ToFollow, ActionZone Zone, Math::Vec3f BasePosition, float Distance, bool IsKiller)
	: Enemy( Parent , IsKiller)
	, ToFollow{ ToFollow }
	, Zone{ Zone }
	, Distance{Distance}
	, BasePosition { BasePosition }
{}

void IntelligentEnemy::Init()
{
	// Get the needed components only once at init
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
	MyCollider = ParentActor->GetComponent<SphereCollider>();
}

void IntelligentEnemy::FixedTick(const float& DeltaTime)
{
	Direction = ToFollow->Position - ParentActor->Transform.Position;
	if (Direction.Norm() < Distance 
		&& ToFollow->Position > Zone.Point1
		&& ToFollow->Position < Zone.Point2) {
		Direction.normalize();
		//MyRigidBody->AddForce(Math::XMVector2PX(Direction.ToXMVector()) * Speed * DeltaTime, ForceMode::Impulse);
		MyRigidBody->SetKinematicTarget(Transform(Vec3f(ParentActor->Transform.Position.x + Direction.x * Speed, ParentActor->Transform.Position.y, ParentActor->Transform.Position.z + Direction.z * Speed),
			Quaternion(atan2f(Direction.x, Direction.z) + physx::PxPi, Vec3f(0, 1, 0))));
	}
	// Return to base
	else 
	{
		// TODO +- 0.1
		if (ParentActor->Transform.Position.MinusByXZ(BasePosition - 0.1f) || ParentActor->Transform.Position.PlusByXZ(BasePosition + 0.1f)) {
			Direction = BasePosition - ParentActor->Transform.Position;
			Direction.normalize();
			MyRigidBody->SetKinematicTarget(Transform(Vec3f(ParentActor->Transform.Position.x + Direction.x * Speed, ParentActor->Transform.Position.y, ParentActor->Transform.Position.z + Direction.z * Speed),
				Quaternion(atan2f(Direction.x, Direction.z) + physx::PxPi, Vec3f(0, 1, 0))));
		}
	}
}

void IntelligentEnemy::SetSpeed(const float& SpeedNew)
{
	Speed = SpeedNew;
}

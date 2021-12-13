#include "stdafx.h"
#include "IntelligentEnemy.h"

#include "render/MoteurWindows.h"
#include "math/Math.h"
#include "math/Vec3f.h"

using namespace Math;

IntelligentEnemy::IntelligentEnemy(Pitbull::Actor* Parent, Math::Transform* ToFollow)
	: Component{ Parent }
	, ToFollow{ ToFollow }
{}

void IntelligentEnemy::Init()
{
	// Get the needed components only once at init
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
}

void IntelligentEnemy::FixedTick(const float& DeltaTime)
{
	Direction = ToFollow->Position - ParentActor->Transform.Position;
	Direction.normalize();
	MyRigidBody->setKinematicTarget(Transform(Vec3f(ParentActor->Transform.Position + Direction * Speed), Quaternion(atan2f(Direction.x, Direction.z) + physx::PxPi, Vec3f(0,1,0))));
}

void IntelligentEnemy::SetSpeed(const float& SpeedNew)
{
	Speed = SpeedNew;
}

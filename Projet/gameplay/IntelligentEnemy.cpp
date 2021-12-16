#include "stdafx.h"
#include "IntelligentEnemy.h"

#include "render/EngineD3D11.h"
#include "math/Math.h"
#include "math/Vec3f.h"

using namespace Math;

IntelligentEnemy::IntelligentEnemy(Pitbull::Actor* Parent, Math::Transform* ToFollow, ActionZone Zone, Math::Vec3f BasePosition, ATerrain* RelativeTerrain,
	Math::Vec3f RelativeTerrainPosition, float Distance, bool IsKiller)
	: Enemy(Parent, IsKiller)
	, ToFollow{ ToFollow }
	, Zone{ Zone }
	, Distance{ Distance }
	, BasePosition{ BasePosition }
	, RelativeTerrain(RelativeTerrain)
	, RelativeTerrainPosition(RelativeTerrainPosition)
{}

void IntelligentEnemy::Init()
{
	// Get the needed components only once at init
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
	MyCollider = ParentActor->GetComponent<SphereCollider>();
}

void IntelligentEnemy::FixedTick(const float& DeltaTime)
{
	RelativeTerrainPosition = ParentActor->Transform.Position - RelativeTerrain->Transform.Position;
	Direction = ToFollow->Position - ParentActor->Transform.Position;
	if (Direction.Norm() < Distance 
		&& ToFollow->Position > Zone.Point1
		&& ToFollow->Position < Zone.Point2) {
		Direction.normalize();
		const float NextPosX = ParentActor->Transform.Position.x + Direction.x * Speed;
		const float NextPosZ = ParentActor->Transform.Position.z + Direction.z * Speed;
		const float NextPosY = RelativeTerrain->GetVertex(static_cast<int>(RelativeTerrainPosition.x / RelativeTerrain->Transform.Scale.x), 
			static_cast<int>(RelativeTerrainPosition.z / RelativeTerrain->Transform.Scale.z)).Position.y * (RelativeTerrain->Transform.Scale.y) + BasePosition.y;

		MyRigidBody->SetKinematicTarget(Transform(Vec3f(NextPosX, NextPosY, NextPosZ),
			Quaternion(atan2f(Direction.x, Direction.z) + physx::PxPi, Vec3f(0, 1, 0))));
	}
	// Return to base
	else 
	{
		if (ParentActor->Transform.Position.MinusByXZ(BasePosition - 0.1f) || ParentActor->Transform.Position.PlusByXZ(BasePosition + 0.1f)) {
			Direction = BasePosition - ParentActor->Transform.Position;
			Direction.normalize();
			const float NextPosX = ParentActor->Transform.Position.x + Direction.x * Speed;
			const float NextPosZ = ParentActor->Transform.Position.z + Direction.z * Speed;
			const float NextPosY = RelativeTerrain->GetVertex(static_cast<int>(RelativeTerrainPosition.x / RelativeTerrain->Transform.Scale.x),
				static_cast<int>(RelativeTerrainPosition.z / RelativeTerrain->Transform.Scale.z)).Position.y * (RelativeTerrain->Transform.Scale.y) + BasePosition.y;
			MyRigidBody->SetKinematicTarget(Transform(Vec3f(NextPosX, NextPosY, NextPosZ),
				Quaternion(atan2f(Direction.x, Direction.z) + physx::PxPi, Vec3f(0, 1, 0))));
		}
	}
}

void IntelligentEnemy::SetSpeed(const float& SpeedNew)
{
	Speed = SpeedNew;
}

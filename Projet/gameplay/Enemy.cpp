#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(Pitbull::Actor* Parent, bool IsKiller, bool BlurEffect)
	: Component{ Parent }
	, IsKiller{ IsKiller }
	, BlurEffect{ BlurEffect }
{
}

void Enemy::Init()
{
	MyCollider = ParentActor->GetComponent<SphereCollider>();
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
}
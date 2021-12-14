#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(Pitbull::Actor* Parent, bool IsKiller)
	: Component{ Parent }
	, IsKiller{ IsKiller }
{
}

void Enemy::Init()
{
	MyCollider = ParentActor->GetComponent<SphereCollider>();
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
}
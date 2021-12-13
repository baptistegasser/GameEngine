#include "stdafx.h"
#include "CheckPoint.h"

#include "core/Actor.h"

CheckPoint::CheckPoint(Pitbull::Actor* Parent)
	: Component{ Parent }
{
}

void CheckPoint::Init()
{
	MyCollider = ParentActor->GetComponent<SphereCollider>();
}

void CheckPoint::Tick(const float& elapsed_time)
{
	
}

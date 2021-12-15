#include "stdafx.h"
#include "Speed.h"
#include "render/MoteurWindows.h"

Speed::Speed(Pitbull::Actor* Parent)
	: Component{ Parent }
{}

void Speed::Init()
{
	// Get the needed components only once at init
	MySprite = ParentActor->GetComponent<TextRenderer>();
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
}

void Speed::FixedTick(const float& DeltaTime)
{
	MySprite->Write(L"Speed : " + std::to_wstring(MyRigidBody->GetVelocity().Norm()) + L" m/s");
}



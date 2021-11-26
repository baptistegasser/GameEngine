#include "stdafx.h"
#include "Plateform.h"
#include "../core/Actor.h"

using namespace physx;

Plateform::Plateform(Pitbull::Actor* Parent, const Point& Departure, const Point& Destination, const bool& Loop)
	: 
	Component{Parent}
	,Departure{Departure}
	, Destination{Destination}
	, Loop {Loop}
{}

void Plateform::Init()
{
	// Get the needed component only once at init
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
}

void Plateform::Tick(const float& DeltaTime)
{
	if (FirstLoop) {

		if (DepartureIsGoal) {
			Direction = Departure - ParentActor->Transform.p;
			if (Point(Speed) <= Direction.abs()) {
				Direction.normalize();
				MyRigidBody->setKinematicTarget(PxTransform(ParentActor->Transform.p + Direction * Speed));
			}
			else {
				DepartureIsGoal = false;
			}
		}
		else {
			Direction = Destination - ParentActor->Transform.p;
			if (Point(Speed) <= Direction.abs()) {
				Direction.normalize();
				MyRigidBody->setKinematicTarget(PxTransform(ParentActor->Transform.p + Direction * Speed));
			}
			else {
				DepartureIsGoal = true;
				if (!Loop)
					FirstLoop = false;
			}
		}
	}
}

void Plateform::SetSpeed(const float& SpeedNew)
{
	Speed = SpeedNew;
}

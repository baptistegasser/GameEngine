#include "stdafx.h"
#include "Plateform.h"
#include "../core/Actor.h"

using namespace physx;

Plateform::Plateform(Pitbull::Actor* Parent, const PxTransform& Departure, const PxTransform& Destination, const bool& Loop)
	: 
	Component{Parent}
	,Departure{Departure}
	, Destination{Destination}
	, FirstLoop {Loop}
{}

void Plateform::Init()
{
	// Get the needed component only once at init
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
}

void Plateform::Tick(const float& DeltaTime)
{
	if (Loop) {

		if (DepartureIsGoal) {
			Direction = Departure.p - ParentActor->Transform.p;
			if (Point(Speed) <= Direction.abs()) {
				Direction.normalize();
				MyRigidBody->setKinematicTarget(PxTransform(PxVec3(ParentActor->Transform.p + Direction * Speed), PxQuat(ParentActor->Transform.q)));
			}
			else {
				DepartureIsGoal = false;
				MyRigidBody->setKinematicTarget(Departure);
			}
		}
		else {
			Direction = Destination.p - ParentActor->Transform.p;
			if (Point(Speed) <= Direction.abs()) {
				Direction.normalize();
				MyRigidBody->setKinematicTarget(PxTransform(PxVec3(ParentActor->Transform.p + Direction * Speed), PxQuat(ParentActor->Transform.q)));
			}
			else {
				DepartureIsGoal = true;
				if (!FirstLoop)
					Loop = false;
				MyRigidBody->setKinematicTarget(Destination);
			}
		}
	}
}

void Plateform::SetSpeed(const float& SpeedNew)
{
	Speed = SpeedNew;
}

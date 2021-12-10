#include "stdafx.h"
#include "Plateform.h"
#include "core/Actor.h"

using namespace Math;

Plateform::Plateform(Pitbull::Actor* Parent, const Transform& Departure, const Transform& Destination, const bool& Loop)
	: Component{Parent}
	, Departure{Departure}
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
			Direction = Departure.Position - ParentActor->Transform.Position;
			if (Vec3f(Speed) <= Direction.abs()) {
				Direction.normalize();
				MyRigidBody->SetKinematicTarget(Transform(Vec3f(ParentActor->Transform.Position + Direction * Speed), ParentActor->Transform.Rotation));
			}
			else {
				DepartureIsGoal = false;
				MyRigidBody->SetKinematicTarget(Departure);
			}
		}
		else {
			Direction = Destination.Position - ParentActor->Transform.Position;
			if (Vec3f(Speed) <= Direction.abs()) {
				Direction.normalize();
				MyRigidBody->SetKinematicTarget(Transform(Vec3f(ParentActor->Transform.Position + Direction * Speed), ParentActor->Transform.Rotation));
			}
			else {
				DepartureIsGoal = true;
				if (!FirstLoop)
					Loop = false;
				MyRigidBody->SetKinematicTarget(Destination);
			}
		}
	}
}

void Plateform::SetSpeed(const float& SpeedNew)
{
	Speed = SpeedNew;
}

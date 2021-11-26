#pragma once

#include "../core/Component.h"
#include "RigidBody.h"
#include "../Point.h"

class Plateform : public Pitbull::Component {
public :
	/// <summary>
	/// Create a kinematic
	/// Move to Departure then Destination, and loop if Loop is true
	/// </summary>
	/// <param name="Parent"></param>
	/// <param name="Departure"></param>
	/// <param name="Destination"></param>
	/// <param name="Loop"></param>
	Plateform(Pitbull::Actor* Parent, const Point& Departure, const Point& Destination, const bool& Loop);
	~Plateform() override = default;

	void Init() override;

	/// <summary>
	/// Update the position of the kinematic
	/// </summary>
	/// <param name="DeltaTime"></param>
	void Tick(const float& DeltaTime) override;

	void SetSpeed(const float& Speed);

private :
	RigidBody* MyRigidBody;

	Point Departure;
	Point Destination;

	bool Loop;
	bool FirstLoop = true;

	bool DepartureIsGoal = true;
	physx::PxVec3 Direction;
	float Speed = 0.1f;
};
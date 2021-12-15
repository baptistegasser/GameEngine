#pragma once

#include "core/Component.h"
#include "physic/RigidBody.h"
#include "math/Vec3f.h"
#include "math/Transform.h"

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
	Plateform(Pitbull::Actor* Parent, const Math::Transform& Departure, const Math::Transform& Destination, const bool& Loop);
	~Plateform() override = default;

	void Init() override;

	/// <summary>
	/// Update the position of the kinematic
	/// </summary>
	/// <param name="DeltaTime"></param>
	void FixedTick(const float& DeltaTime) override;

	void SetSpeed(const float& Speed);

private :
	RigidBody* MyRigidBody;

	const Math::Transform Departure;
	const Math::Transform Destination;

	bool Loop = true;
	const bool FirstLoop;

	bool DepartureIsGoal = true;
	Math::Vec3f Direction;
	float Speed = 0.1f;
};
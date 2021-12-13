#pragma once

#include "core/Component.h"
#include "physic/RigidBody.h"

class IntelligentEnemy : public Pitbull::Component
{
public :
	IntelligentEnemy(Pitbull::Actor* Parent, Math::Transform* ToFollow);
	~IntelligentEnemy() override = default;

	void Init() override;

	/// <summary>
	/// Update the direction of the player
	/// Update the type of camera
	/// </summary>
	void FixedTick(const float& DeltaTime) override;

	void SetSpeed(const float& SpeedNew);

private:
	RigidBody* MyRigidBody;
	Math::Transform* ToFollow;
	Math::Vec3f Direction;

	float Speed = 0.1f;
};


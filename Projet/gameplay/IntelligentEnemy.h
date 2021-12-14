#pragma once

#include "core/Component.h"
#include "physic/RigidBody.h"
#include <limits>

class IntelligentEnemy : public Pitbull::Component
{
public :
	IntelligentEnemy(Pitbull::Actor* Parent, Math::Transform* ToFollow, float Distance = std::numeric_limits<float>::infinity());
	~IntelligentEnemy() override = default;

	void Init() override;

	/// <summary>
	/// Update the position and rotation of the enemy
	/// Follow the actor's transform only if he is at a distance under Distance
	/// </summary>
	void FixedTick(const float& DeltaTime) override;

	void SetSpeed(const float& SpeedNew);

private:
	RigidBody* MyRigidBody;
	Math::Transform* ToFollow;
	Math::Vec3f Direction;

	float Speed = 0.1f;
	float Distance;
};


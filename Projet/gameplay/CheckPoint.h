#pragma once

#include "core/Component.h"
#include "physic/CapsuleCollider.h"
#include "physic/RigidBody.h"
#include "physic/SphereCollider.h"

class CheckPoint : public Pitbull::Component
{
public:
	CheckPoint(Pitbull::Actor* Parent);
	~CheckPoint() override = default;

	void Init() override;

	/// <summary>
	/// Update the death state
	/// </summary>
	void Tick(const float& ElapsedTime) override;

private:
	CapsuleCollider* MyCollider;
	RigidBody* MyRigidBody;
};

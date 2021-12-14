#pragma once

#include "core/Component.h"
#include "physic/CapsuleCollider.h"
#include "physic/RigidBody.h"
#include "physic/SphereCollider.h"

class CheckPoint : public Pitbull::Component
{
public:
	/// <summary>
	/// Create a functional CheckPoint 
	/// </summary>
	/// <param name="Parent"></param>
	CheckPoint(Pitbull::Actor* Parent);
	~CheckPoint() override = default;

	void Init() override;

	// Check Point is activate
	bool IsVisited = false;
private:
	CapsuleCollider* MyCollider;
	RigidBody* MyRigidBody;
};

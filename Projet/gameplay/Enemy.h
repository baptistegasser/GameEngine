#pragma once

#include "core/Component.h"
#include "physic/SphereCollider.h"
#include "physic/RigidBody.h"

class Enemy : public Pitbull::Component
{
public:
	/// <summary>
	/// Create a functional CheckPoint 
	/// </summary>
	/// <param name="Parent"></param>
	Enemy(Pitbull::Actor* Parent, bool IsKiller = false, bool BlurEffect = false);
	~Enemy() override = default;

	void Init() override;

	// Check Point is activate
	bool IsKiller = false;
	bool BlurEffect;
protected:
	SphereCollider* MyCollider;
	RigidBody* MyRigidBody;
};

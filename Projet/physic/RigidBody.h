#pragma once

#include "core/Component.h"
#include "PxPhysicsAPI.h"

class RigidBody : public Pitbull::Component {
public:
	~RigidBody() override;

	void Init() override;
	void Tick(const float& ElapsedTime) override;

protected:
	friend class Pitbull::Actor;
	RigidBody(Pitbull::Actor* Parent, bool IsStatic, bool DisableGravity, float Mass, physx::PxVec3 Velocity = physx::PxVec3{ 0.f });

private:
	bool IsStatic = true;
	bool DisableGravity = false;
	float Mass = 0.f;
	physx::PxVec3 Velocity{ 0.f, 0.f, 0.f };

	physx::PxRigidActor* RigidActor;
};

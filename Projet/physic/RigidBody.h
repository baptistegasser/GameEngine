#pragma once

#include "core/Component.h"
#include "PxPhysicsAPI.h"

class RigidBody : public Pitbull::Component {
public:
	RigidBody() = default;
	~RigidBody() = default;

	void Init() override;

private:
	bool UseGravity = false;
	bool IsStatic = true;
	float Mass = 0.f;
	physx::PxVec3 Velocity{ 0.f, 0.f, 0.f };

	physx::PxRigidActor* RigidActor;
};

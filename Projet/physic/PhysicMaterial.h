#pragma once

#include "PxPhysicsAPI.h"

struct PhysicMaterial {
	float StaticFriction, DynamicFriction, Restitution;

	physx::PxMaterial* GetPxMaterial() const;
};

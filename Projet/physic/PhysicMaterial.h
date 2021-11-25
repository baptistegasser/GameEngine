#pragma once

#include "PxPhysicsAPI.h"

class PhysicMaterial {
public:
	PhysicMaterial(float StaticFriction, float DynamicFriction, float Restitution);
	~PhysicMaterial() = default;;
	physx::PxMaterial* GetPxMaterial();

private:
	physx::PxMaterial* Material;
	float StaticFriction, DynamicFriction, Restitution;
};

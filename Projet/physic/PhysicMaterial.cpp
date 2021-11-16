#include "stdafx.h"

#include "physic/PhysicManager.h"
#include "PhysicMaterial.h"

physx::PxMaterial* PhysicMaterial::GetPxMaterial() const
{
	static physx::PxMaterial* Mat = PhysicManager::GetInstance().Physics->createMaterial(StaticFriction, DynamicFriction, Restitution);
	return Mat;
}

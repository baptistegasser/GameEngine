#include "stdafx.h"
#include "PhysicMaterial.h"

#include "physic/PhysicManager.h"

PhysicMaterial::PhysicMaterial(float StaticFriction, float DynamicFriction, float Restitution)
	: StaticFriction{ StaticFriction }
	, DynamicFriction{ DynamicFriction }
	, Restitution{ Restitution }
	, Material{ nullptr }
{}

physx::PxMaterial* PhysicMaterial::GetPxMaterial()
{
	if (!Material) {
		Material = PhysicManager::GetInstance().Physics->createMaterial(StaticFriction, DynamicFriction, Restitution);
	}
	return Material;
}

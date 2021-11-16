#pragma once

#include "core/Component.h"
#include "PhysicMaterial.h"
#include "physic/PhysicManager.h"

#include "PxPhysicsAPI.h"

class Collider : public Pitbull::Component {
public:
	Collider(const PhysicMaterial& Material);

	physx::PxGeometry* GetPxGeometry() const
	{
		static physx::PxGeometry* Geometry = GetGeometryImpl();
		return Geometry;
	}

	physx::PxMaterial* GetPxMaterial() const
	{
		return Material.GetPxMaterial();
	}

protected:
	virtual physx::PxGeometry* GetGeometryImpl() const = 0;

private:
	PhysicMaterial Material;
};

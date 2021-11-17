#pragma once

#include "core/Component.h"
#include "PhysicMaterial.h"
#include "physic/PhysicManager.h"

#include "PxPhysicsAPI.h"

class Collider : public Pitbull::Component {
public:
	Collider(const PhysicMaterial& Material);
	~Collider();

	physx::PxGeometry* GetPxGeometry();
	physx::PxMaterial* GetPxMaterial();

protected:
	virtual physx::PxGeometry* GetGeometryImpl() const = 0;

private:
	physx::PxGeometry* Geometry;
	PhysicMaterial Material;
};

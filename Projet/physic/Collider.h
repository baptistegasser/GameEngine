#pragma once

#include "core/Component.h"
#include "PhysicMaterial.h"
#include "physic/PhysicManager.h"

#include "PxPhysicsAPI.h"

class Collider : public Pitbull::Component {
public:
	~Collider() override = default;

	physx::PxGeometry* GetPxGeometry();
	physx::PxMaterial* GetPxMaterial();

protected:
	Collider(Pitbull::Actor* Parent, const PhysicMaterial& Material);
	virtual physx::PxGeometry&& GetGeometryImpl() const = 0;

private:
	std::unique_ptr<physx::PxGeometry> Geometry;
	PhysicMaterial Material;
};

#pragma once

#include "Collider.h"

class BoxCollider : public Collider {
public:
	physx::PxGeometry&& GetGeometryImpl() const override;

	const physx::PxVec3 HalfExtent;

protected:
	explicit BoxCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const physx::PxVec3 HalfExtent);
};

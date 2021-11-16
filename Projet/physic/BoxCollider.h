#pragma once

#include "Collider.h"

class BoxCollider : public Collider {
public:
	explicit BoxCollider(const PhysicMaterial& Material, const physx::PxVec3 HalfExtent);
	physx::PxGeometry* GetGeometryImpl() const override;

	const physx::PxVec3 HalfExtent;
};

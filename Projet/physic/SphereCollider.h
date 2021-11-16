#pragma once

#include "Collider.h"

class SphereCollider : public Collider {
public:
	explicit SphereCollider(const PhysicMaterial& Material, const float Radius);
	physx::PxGeometry* GetGeometryImpl() const override;

	const float Radius;
};

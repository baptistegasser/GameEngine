#pragma once

#include "Collider.h"

class CapsuleCollider : public Collider {
public:
	explicit CapsuleCollider(const PhysicMaterial& Material, const float Radius, const float HalfHeight);
	physx::PxGeometry* GetGeometryImpl() const override;

	const float Radius;
	const float HalfHeight;
};

#pragma once

#include "Collider.h"

class SphereCollider : public Collider {
public:
	physx::PxGeometry&& GetGeometryImpl() const override;

	const float Radius;

protected:
	explicit SphereCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const float Radius);
};

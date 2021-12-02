#pragma once

#include "Collider.h"

class SphereCollider : public Collider {
public:
	physx::PxGeometry* GetGeometryImpl(const Math::Vec3f& Scale) const noexcept override;

	const float Radius;

protected:
	friend class Pitbull::Actor;
	explicit SphereCollider(Pitbull::Actor* Parent, const float Radius = 1.f, const PhysicMaterial& Material = DefaultMaterial, const Math::Vec3f& Offset = DefaultOffset);
};

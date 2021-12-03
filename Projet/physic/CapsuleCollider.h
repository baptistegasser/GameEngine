#pragma once

#include "Collider.h"

class CapsuleCollider : public Collider {
public:
	physx::PxGeometry* GetGeometryImpl(const Math::Vec3f& Scale) const noexcept override;

	const float Radius;
	const float HalfHeight;

protected:
	friend class Pitbull::Actor;
	explicit CapsuleCollider(Pitbull::Actor* Parent, const float Radius = 0.5f, const float HalfHeight = 1.0f, const PhysicMaterial& Material = DefaultMaterial, const Math::Vec3f& Offset = DefaultOffset);
};

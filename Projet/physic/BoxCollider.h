#pragma once

#include "Collider.h"
#include "math/Vec3f.h"

class BoxCollider : public Collider {
public:
	physx::PxGeometry* GetGeometryImpl(const Math::Vec3f& Scale) const noexcept override;

	const Math::Vec3f HalfExtent;

protected:
	friend class Pitbull::Actor;
	explicit BoxCollider(Pitbull::Actor* Parent, const Math::Vec3f HalfExtent = {1.f}, const PhysicMaterial& Material = DefaultMaterial, const Math::Vec3f& Offset = DefaultOffset);
};

#pragma once

#include "Collider.h"

class CapsuleCollider : public Collider {
public:
	physx::PxGeometry* GetGeometryImpl() const override;

	const float Radius;
	const float HalfHeight;

protected:
	friend class Pitbull::Actor;
	explicit CapsuleCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const float Radius, const float HalfHeight);
};

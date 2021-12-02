#pragma once
#include "Collider.h"

class ATerrain;

class HeightFieldCollider : public Collider {
public:
	[[nodiscard]] physx::PxGeometry* GetGeometryImpl(const Math::Vec3f& Scale) const noexcept override;

protected:
	friend class Pitbull::Actor;
	explicit HeightFieldCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const ATerrain* Terrain);

	physx::PxHeightField* HeightField;
};


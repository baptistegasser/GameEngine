#pragma once
#include "Collider.h"

class ATerrain;

class HeightFieldCollider : public Collider {
public:
	[[nodiscard]] physx::PxGeometry* GetGeometryImpl() const override;

protected:
	friend class Pitbull::Actor;
	explicit HeightFieldCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const ATerrain* Terrain);

	physx::PxHeightField* HeightField;

	float RowScale;
	float HeightScale;
	float ColumnScale;
};


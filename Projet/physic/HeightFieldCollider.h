#pragma once
#include "Collider.h"

class Landscape;

class HeightFieldCollider : public Collider {
public:
	physx::PxGeometry* GetGeometryImpl() const override;

protected:
	friend class Pitbull::Actor;
	explicit HeightFieldCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const Landscape* Landscape);

	physx::PxHeightFieldSample* Samples;
	physx::PxHeightField* HeightField;

	float HeightScale;
	float ColumnScale;
	float RowScale;
};


#include "stdafx.h"
#include "Collider.h"

Collider::Collider(const PhysicMaterial& Material)
	: Geometry{ nullptr }
	, Material{ Material }
{}

Collider::~Collider()
{
	delete Geometry;
	Geometry = nullptr;
}

physx::PxGeometry* Collider::GetPxGeometry()
{
	if (!Geometry) {
		Geometry = GetGeometryImpl();
	}
	return Geometry;
}

physx::PxMaterial* Collider::GetPxMaterial()
{
	return Material.GetPxMaterial();
}

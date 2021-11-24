#include "stdafx.h"
#include "Collider.h"

Collider::Collider(Pitbull::Actor* Parent, const PhysicMaterial& Material)
	: Component{ Parent }
	, Geometry{ nullptr }
	, Material{ Material }
{
	TypeFlags |= PHYSIC_COMPONENT;
}

physx::PxGeometry* Collider::GetPxGeometry()
{
	if (!Geometry) {
		Geometry = std::unique_ptr<physx::PxGeometry>(GetGeometryImpl());
	}
	return Geometry.get();
}

physx::PxMaterial* Collider::GetPxMaterial()
{
	return Material.GetPxMaterial();
}

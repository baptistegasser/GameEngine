#include "stdafx.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(const PhysicMaterial& Material, const physx::PxVec3 HalfExtent)
	: Collider{ Material }
	, HalfExtent{ HalfExtent }
{}

physx::PxGeometry* BoxCollider::GetGeometryImpl() const
{
	return new physx::PxBoxGeometry{ HalfExtent };
}

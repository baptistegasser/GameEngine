#include "stdafx.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const physx::PxVec3 HalfExtent)
	: Collider{ Parent, Material }
	, HalfExtent{ HalfExtent }
{}

physx::PxGeometry&& BoxCollider::GetGeometryImpl() const
{
	return std::move(physx::PxBoxGeometry{ HalfExtent });
}
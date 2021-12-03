#include "stdafx.h"
#include "BoxCollider.h"

BoxCollider::BoxCollider(Pitbull::Actor* Parent, const Math::Vec3f HalfExtent, const PhysicMaterial& Material, const Math::Vec3f& Offset)
	: Collider{ Parent, Material, Offset }
	, HalfExtent{ HalfExtent }
{}

physx::PxGeometry* BoxCollider::GetGeometryImpl(const Math::Vec3f& Scale) const noexcept
{
	return new physx::PxBoxGeometry{ Scale.Scale(HalfExtent) };
}

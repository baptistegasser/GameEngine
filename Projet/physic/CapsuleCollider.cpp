#include "stdafx.h"
#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const float Radius, const float HalfHeight)
	: Collider{ Parent, Material }
	, Radius{ Radius }
	, HalfHeight{ HalfHeight }
{}

physx::PxGeometry&& CapsuleCollider::GetGeometryImpl() const
{
	return std::move(physx::PxCapsuleGeometry{ Radius , HalfHeight });
}
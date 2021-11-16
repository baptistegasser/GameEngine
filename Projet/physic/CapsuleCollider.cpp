#include "stdafx.h"
#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider(const PhysicMaterial& Material, const float Radius, const float HalfHeight)
	: Collider{ Material }
	, Radius{ Radius }
	, HalfHeight{ HalfHeight }
{}

physx::PxGeometry* CapsuleCollider::GetGeometryImpl() const
{
	return new physx::PxCapsuleGeometry{ Radius , HalfHeight};
}
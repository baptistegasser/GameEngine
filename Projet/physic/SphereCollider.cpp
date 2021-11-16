#include "stdafx.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider(const PhysicMaterial& Material, const float Radius)
	: Collider { Material }
	, Radius{ Radius }
{}

physx::PxGeometry* SphereCollider::GetGeometryImpl() const
{
	return new physx::PxSphereGeometry{ Radius };
}

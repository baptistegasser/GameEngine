#include "stdafx.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const float Radius)
	: Collider { Parent, Material }
	, Radius{ Radius }
{}

physx::PxGeometry* SphereCollider::GetGeometryImpl() const
{
	return new physx::PxSphereGeometry{ Radius };
}

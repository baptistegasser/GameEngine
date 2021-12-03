#include "stdafx.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider(Pitbull::Actor* Parent, const float Radius, const PhysicMaterial& Material, const Math::Vec3f& Offset)
	: Collider{ Parent, Material, Offset }
	, Radius{ Radius }
{}

physx::PxGeometry* SphereCollider::GetGeometryImpl(const Math::Vec3f& Scale) const noexcept
{
	// We need to take one axis to scale the radius
	// Opionated choice : use x axis
	return new physx::PxSphereGeometry{ Radius * Scale.x };
}

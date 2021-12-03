#include "stdafx.h"
#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider(Pitbull::Actor* Parent, const float Radius, const float HalfHeight, const PhysicMaterial& Material, const Math::Vec3f& Offset)
	: Collider{ Parent, Material, Offset }
	, Radius{ Radius }
	, HalfHeight{ HalfHeight }
{}

physx::PxGeometry* CapsuleCollider::GetGeometryImpl(const Math::Vec3f& Scale) const noexcept
{
	// We need to take either the x or z axis to scale the radius
	// Opionated choice : use x axis
	return new physx::PxCapsuleGeometry{ Radius * Scale.x, HalfHeight * Scale.y };
}

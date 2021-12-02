#include "stdafx.h"
#include "Collider.h"

#include "util/Util.h"

const Math::Vec3f Collider::DefaultOffset{ 0.f };
const PhysicMaterial Collider::DefaultMaterial{ 0.6f, 0.6f, 0.f };

Collider::Collider(Pitbull::Actor* Parent, const PhysicMaterial& Material, const Math::Vec3f& Offset)
	: Component{Parent}
	, Offset{ Offset }
	, Material{ Material }
{}

physx::PxGeometry* Collider::GetPxGeometry(const Math::Vec3f& Scale) noexcept
{
	return GetGeometryImpl(Scale);
}

physx::PxMaterial* Collider::GetPxMaterial() noexcept
{
	return Material.GetPxMaterial();
}

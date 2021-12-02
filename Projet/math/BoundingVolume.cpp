#include "stdafx.h"
#include "BoundingVolume.h"

bool Intersect::operator()(const BoundingBox& V0, const BoundingBox& V1) const
{
	return false;
}

bool Intersect::operator()(const BoundingSphere& V0, const BoundingSphere& V1) const
{
    return false;
}

bool Intersect::operator()(const BoundingBox& Rect, const BoundingSphere& Sphere) const
{
    float sphereXDistance = abs(Sphere.Center.x - Rect.Center.x);
    float sphereYDistance = abs(Sphere.Center.y - Rect.Center.y);
    float sphereZDistance = abs(Sphere.Center.z - Rect.Center.z);

    float Width = Rect.HalfWidth*2.f
		, Height = Rect.HalfHeight*2.f
		, Depth = Rect.HalfDepth*2.f;

    if (sphereXDistance >= (Width  + Sphere.Radius)) { return false; }
    if (sphereYDistance >= (Height + Sphere.Radius)) { return false; }
    if (sphereZDistance >= (Depth  + Sphere.Radius)) { return false; }

    if (sphereXDistance < (Width)) { return true; }
    if (sphereYDistance < (Height)) { return true; }
    if (sphereZDistance < (Depth)) { return true; }

    float cornerDistance_sq = ((sphereXDistance - Width) * (sphereXDistance - Width)) +
        ((sphereYDistance - Height) * (sphereYDistance - Height) +
            ((sphereYDistance - Depth) * (sphereYDistance - Depth)));

    return (cornerDistance_sq < (Sphere.Radius * Sphere.Radius));
}

bool Intersect::operator()(const BoundingSphere& V0, const BoundingBox& V1) const
{
    return operator()(V1, V0);
}

bool Contains::operator()(const BoundingBox& Rect) const
{
    return -Rect.HalfWidth  <= Pos.x && Pos.x <= Rect.HalfWidth
        && -Rect.HalfHeight <= Pos.y && Pos.y <= Rect.HalfHeight
        && -Rect.HalfDepth  <= Pos.z && Pos.z <= Rect.HalfDepth;
}

bool Contains::operator()(const BoundingSphere& Sphere) const
{
    return Pos.Distance(Sphere.Center) <= Sphere.Radius;
}

bool VolumeContains(const BoundingVolume& Volume, const Math::Vec3f& Pos)
{
    return std::visit(Contains{Pos}, Volume);
}

bool VolumesIntersect(const BoundingVolume& V0, const BoundingVolume& V1)
{
	return std::visit(Intersect{}, V0, V1);
}

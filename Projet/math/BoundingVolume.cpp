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
    return -Rect.HalfWidth  <= Point.x && Point.x <= Rect.HalfWidth
        && -Rect.HalfHeight <= Point.y && Point.y <= Rect.HalfHeight
        && -Rect.HalfDepth  <= Point.z && Point.z <= Rect.HalfDepth;
}

bool Contains::operator()(const BoundingSphere& Sphere) const
{
    return sqrtf(
        powf(Point.x - Sphere.Center.x, 2.f) +
        powf(Point.y - Sphere.Center.y, 2.f) +
        powf(Point.z - Sphere.Center.z, 2.f)
    ) <= Sphere.Radius;
}

bool VolumeContains(const BoundingVolume& Volume, const Point& Point)
{
    return std::visit(Contains{Point}, Volume);
}

bool VolumesIntersect(const BoundingVolume& V0, const BoundingVolume& V1)
{
	return std::visit(Intersect{}, V0, V1);
}

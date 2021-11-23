#include "stdafx.h"
#include "BoundingVolume.h"

bool Intersect::operator()(const BoundingBox& V0, const BoundingBox& V1) const
{
}

bool Intersect::operator()(const BoundingBox& V0, const BoundingSphere& V1) const
{
}
bool Intersect::operator()(const BoundingSphere& V0, const BoundingSphere& V1) const
{
}

bool VolumesIntersect(const BoundingVolume& V0, const BoundingVolume& V1)
{
	return std::visit(Intersect{}, V0, V1);
}

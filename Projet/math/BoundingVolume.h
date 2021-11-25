﻿#pragma once

#include "BoundingBox.h"
#include "BoundingSphere.h"

#include <variant>


using BoundingVolume = std::variant<BoundingBox, BoundingSphere>;

struct Intersect
{
	bool operator()(const BoundingBox& V0, const BoundingBox& V1) const;
	bool operator()(const BoundingSphere& V0, const BoundingSphere& V1) const;
	bool operator()(const BoundingBox& V0, const BoundingSphere& V1) const;
	bool operator()(const BoundingSphere& V0, const BoundingBox& V1) const;
};

struct Contains
{
	const Point Point;

	bool operator()(const BoundingBox& Rect) const;
	bool operator()(const BoundingSphere& Sphere) const;
};

bool VolumeContains(const BoundingVolume& Volume, const Point& Point);
bool VolumesIntersect(const BoundingVolume& V0, const BoundingVolume& V1);
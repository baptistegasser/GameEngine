#pragma once

#include "Point.h"

/// <summary>
/// Structure representing a 3D Box.
/// </summary>
struct BoundingBox
{
	float HalfWidth, HalfHeight, HalfDepth;
	// The center of this box
	Point Center;

	BoundingBox(float A, Point Center = Point{ 0.f });
	BoundingBox(float HalfWidth, float HalfHeight, float HalfDepth, Point Center = Point{ 0.f });

	bool inline ContainPoint(const Point& p) const noexcept;
};
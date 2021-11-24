#pragma once

#include "Point.h"

/// <summary>
/// Structure representing a 3D Box.
/// </summary>
struct BoundingSphere
{
	const float Radius;
	// The center of this box
	const Point Center;

	BoundingSphere(float Radius, Point Center = Point{ 0.f });
};
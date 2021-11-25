#pragma once

#include "Point.h"

/// <summary>
/// Structure representing a 3D Box.
/// </summary>
struct BoundingSphere
{
	float Radius;
	// The center of this box
	Point Center;

	BoundingSphere(float Radius, Point Center = Point{ 0.f });
};
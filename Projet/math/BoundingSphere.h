#pragma once

#include "math/Vec3f.h"

/// <summary>
/// Structure representing a 3D Box.
/// </summary>
struct BoundingSphere
{
	float Radius;
	// The center of this box
	Math::Vec3f Center;

	BoundingSphere(float Radius, Math::Vec3f Center = { 0.f });
};
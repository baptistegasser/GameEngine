#pragma once

#include "math/Vec3f.h"

/// <summary>
/// Structure representing a 3D Box.
/// </summary>
struct BoundingBox
{
	float HalfWidth, HalfHeight, HalfDepth;
	// The center of this box
	Math::Vec3f Center;

	BoundingBox(float A, Math::Vec3f Center = { 0.f });
	BoundingBox(float HalfWidth, float HalfHeight, float HalfDepth, Math::Vec3f Center = { 0.f });
};
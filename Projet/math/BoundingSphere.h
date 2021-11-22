#pragma once

#include "Boundingvolume.h"

/// <summary>
/// Structure representing a 3D Box.
/// </summary>
class BoundingSphere : public BoundingVolume
{
	const float Diameter;

public:
	const float Radius;
	// The center of this box
	const Point Center;

	BoundingSphere(float Radius, Point Center = Point{ 0.f });

	bool inline ContainPoint(const Point& p) noexcept override;
};
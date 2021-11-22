﻿#pragma once

#include "Boundingvolume.h"

/// <summary>
/// Structure representing a 3D Box.
/// </summary>
class BoundingBox : public BoundingVolume
{
public:
	const float HalfWidth, HalfHeight, HalfDepth;
	// The center of this box
	const Point Center;

	BoundingBox(float A, Point Center = Point{ 0.f });
	BoundingBox(float HalfWidth, float HalfHeight, float HalfDepth, Point Center = Point{ 0.f });

	bool inline ContainPoint(const Point& p) noexcept override;
};
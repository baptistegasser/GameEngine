#pragma once

#include "Point.h"

struct BoundingVolume {
	virtual bool ContainPoint(const Point& p) noexcept
	{
		return false;
	}
};

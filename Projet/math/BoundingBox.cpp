#include "stdafx.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox(float A, Point Center)
	: BoundingBox{ A, A, A, Center }
{}

BoundingBox::BoundingBox(float HalfWidth, float HalfHeight, float HalfDepth, Point Center)
	: HalfWidth{ HalfWidth }
	, HalfHeight{ HalfHeight }
	, HalfDepth{ HalfDepth }
	, Center{ Center }
{}

bool BoundingBox::ContainPoint(const Point& p) const noexcept
{
	return -HalfWidth <= p.x && p.x <= HalfWidth
		&& -HalfHeight <= p.y && p.y <= HalfHeight
		&& -HalfDepth <= p.z && p.z <= HalfDepth;
}
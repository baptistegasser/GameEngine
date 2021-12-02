#include "stdafx.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox(float A, Math::Vec3f Center)
	: BoundingBox{ A, A, A, Center }
{}

BoundingBox::BoundingBox(float HalfWidth, float HalfHeight, float HalfDepth, Math::Vec3f Center)
	: HalfWidth{ HalfWidth }
	, HalfHeight{ HalfHeight }
	, HalfDepth{ HalfDepth }
	, Center{ Center }
{}

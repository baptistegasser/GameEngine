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

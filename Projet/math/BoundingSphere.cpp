#include "stdafx.h"
#include "BoundingSphere.h"

BoundingSphere::BoundingSphere(float Radius, Point Center)
	: Diameter{ Radius*2 }
	, Radius{ Radius }
	, Center{ Center }
{}

bool BoundingSphere::ContainPoint(const Point& p) noexcept
{
	return (
			std::powf(p.x - Center.x, 2.0f)
		  + std::powf(p.y - Center.y, 2.0f)
		  + std::powf(p.z - Center.z, 2.0f)
		) < std::powf(Diameter, 2.0f);
}

#pragma once

/// <summary>
/// Simple representation of a point in 3D space
/// </summary>
struct Point : public physx::PxVec3 {
	Point(float a)
		: PxVec3{ a }
	{}
	Point(float x, float y, float z)
		: PxVec3{ x, y, z }
	{}
	Point(const physx::PxTransform& Transform)
		: PxVec3{ Transform.p }
	{}
	Point operator*(const float& Factor) const noexcept
	{
		return { x * Factor, y * Factor, z * Factor };
	}
	Point& operator+=(const Point& lhr) noexcept
	{
		this->x += lhr.x;
		this->y += lhr.y;
		this->z += lhr.z;
		return *this;
	}
};

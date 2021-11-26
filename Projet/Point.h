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
	Point(const PxVec3& Vec3)
		: PxVec3{ Vec3 }
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
	bool operator==(const physx::PxTransform& Transform) const noexcept
	{
		return this->x == Transform.p.x
			&& this->y == Transform.p.y
			&& this->z == Transform.p.z;
	}
	bool operator!=(const physx::PxTransform& Transform) const noexcept
	{
		return !(*this == Transform);
	}
	bool operator<=(const physx::PxVec3& Vec3) const noexcept
	{
		return this->x <= Vec3.x
			&& this->y <= Vec3.y
			&& this->z <= Vec3.z;
	}

	float Distance(const Point& Other) const noexcept
	{
		return sqrtf(
			powf(x - Other.x, 2.f) +
			powf(y - Other.y, 2.f) +
			powf(z - Other.z, 2.f)
		);
	}
};

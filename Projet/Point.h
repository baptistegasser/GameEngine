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
};

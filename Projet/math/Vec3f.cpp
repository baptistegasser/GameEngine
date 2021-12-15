#include "stdafx.h"
#include "Vec3f.h"

namespace Math
{
	Vec3f::Vec3f(const float val) noexcept
		: physx::PxVec3{ val }
	{}

	Vec3f::Vec3f(const float x, const float y, const float z) noexcept
		: physx::PxVec3{ x, y, z }
	{}

	Vec3f::Vec3f(const PxVec3& Vector) noexcept
		: physx::PxVec3{ Vector }
	{}

	Vec3f::Vec3f(const DirectX::XMVECTOR& Vector) noexcept
		: physx::PxVec3{ Vector.vector4_f32[0], Vector.vector4_f32[1], Vector.vector4_f32[2] }
	{}

	DirectX::XMVECTOR Vec3f::ToXMVector() const noexcept
	{
		return DirectX::XMVECTOR{ x, y, z };
	}

	float Vec3f::Distance(const Vec3f& Vec) const noexcept
	{
		const auto Diff = *this - Vec;
		return sqrtf(
			powf(Diff.x, 2.f) +
			powf(Diff.y, 2.f) +
			powf(Diff.z, 2.f)
		);
	}

	Vec3f Vec3f::Scale(const float Factor) const noexcept
	{
		return *this * Factor;
	}

	Vec3f Vec3f::Scale(const Vec3f& Scaler) const noexcept
	{
		return Vec3f{ x * Scaler.x, y * Scaler.y, z * Scaler.z };
	}

	float Vec3f::Norm() const noexcept
	{
		return sqrtf(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2));
	}

	bool Vec3f::operator<=(const Vec3f& Vec) const noexcept
	{
		return this->x <= Vec.x
			|| this->y <= Vec.y
			|| this->z <= Vec.z;
	}
}
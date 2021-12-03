#include "stdafx.h"
#include "Math.h"

namespace Math
{
	float Deg2Rad(const float degrees) noexcept
	{
		return degrees * (MATH_PI / 180.f);
	}

	float Rad2Deg(const float radians) noexcept
	{
		return radians * (180.f / MATH_PI);
	}

	DirectX::XMVECTOR PX2XMVector(const physx::PxVec3& Vec) noexcept
	{
		return DirectX::XMVECTOR{ Vec.x, Vec.y, Vec.z };
	}

	physx::PxVec3 XMVector2PX(const DirectX::XMVECTOR& Vec) noexcept
	{
		return physx::PxVec3{DirectX::XMVectorGetX(Vec), DirectX::XMVectorGetY(Vec), DirectX::XMVectorGetZ(Vec)};
	}
} // namespace Math
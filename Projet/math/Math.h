#pragma once

#include "PxPhysicsAPI.h"
#include <DirectXMath.h>

namespace Math
{
	constexpr float MATH_PI = 3.14159265359f;

	/// <summary>
	/// Convert an angle in degrees to the matching radians value.
	/// </summary>
	float Deg2Rad(const float degrees) noexcept;
	/// <summary>
	/// Convert an angle in radians to the matching degrees value.
	/// </summary>
	float Rad2Deg(const float radians) noexcept;

	DirectX::XMVECTOR PX2XMVector(const physx::PxVec3& Vec) noexcept;

	physx::PxVec3 XMVector2PX (const DirectX::XMVECTOR& Vec) noexcept;
} // namespace Math
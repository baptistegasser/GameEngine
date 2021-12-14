#pragma once

#include "Vec3f.h"

#include "PxPhysicsAPI.h"
#include <DirectXMath.h>

namespace Math
{
	/// <summary>
	/// Own Quaternion structure to hide usage of physx and provide utility functions
	/// notably to bridge between physx and directx data structures.
	/// </summary>
	struct Quaternion : public physx::PxQuat
	{
		Quaternion() noexcept;
		Quaternion(const float a, const Vec3f& Axis) noexcept;
		Quaternion(const float x, const float y, const float z, const float w) noexcept;
		/// <summary>
		/// Create a quaternion from angle values.
		/// </summary>
		/// <param name="pitch">Rotation on the X axis in degrees</param>
		/// <param name="yaw">Rotation on the Y axis in degrees</param>
		/// <param name="roll">Rotation on the Z axis in degrees</param>
		Quaternion(const float pitch, const float yaw, const float roll) noexcept;

		/// <summary>
		/// Convert this quaternion to matching euleur angles in degrees.
		/// </summary>
		/// <returns>A vector containing the x, y and z euler angles</returns>
		Vec3f ToEuler() const noexcept;
		/// <summary>
		/// Convert this quaternion to a directx matrix.
		/// </summary>
		DirectX::XMMATRIX ToXMMatrix() const noexcept;

		/// <summary>
		/// Allow to copy value from a physx quaternion.
		/// </summary>
		void operator=(const physx::PxQuat& Quaternion) noexcept;
	};
}

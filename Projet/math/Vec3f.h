#pragma once

#include "PxPhysicsAPI.h"
#include <DirectXMath.h>

namespace Math
{
	/// <summary>
	/// Own vector structure to hide usage of physx and provide utility functions
	/// notably to bridge between physx and directx data structures.
	/// </summary>
	struct Vec3f : public physx::PxVec3
	{
		Vec3f() = default;
		Vec3f(const float val) noexcept;
		Vec3f(const float x, const float y, const float z) noexcept;
		Vec3f(const PxVec3& Vector) noexcept;
		Vec3f(const DirectX::XMVECTOR& Vector) noexcept;

		/// <summary>
		/// Convert to a DirectX::XMVector;
		/// </summary>
		DirectX::XMVECTOR ToXMVector() const noexcept;
		/// <summary>
		/// Calculate the distance between this and the target vector.
		/// </summary>
		float Distance(const Vec3f& Vec) const noexcept;
		/// <summary>
		/// Scale by a given factor.
		/// </summary>
		/// <returns>A Vec3f matching this vector scaled.</returns>
		Vec3f Scale(const float Factor) const noexcept;
		/// <summary>
		/// Scale by another Vec3f.
		/// </summary>
		/// <returns>A Vec3f matching this vector scaled.</returns>
		Vec3f Scale(const Vec3f& Scaler) const noexcept;
		/// <summary>
		/// Calculate the Norm of the vector.
		/// </summary>
		/// <returns> The Norm </returns>
		float Norm() const noexcept;

		bool operator<=(const Vec3f& Vec) const noexcept;
	};
}
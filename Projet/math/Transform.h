#pragma once

#include "Vec3f.h"
#include "Quaternion.h"

namespace Math
{
	/// <summary>
	/// Represent the placement, scale and rotation of an Object.
	/// </summary>
	struct Transform {
		/// <summary>
		/// Position in 3D space, default to origin.
		/// </summary>
		Math::Vec3f Position{ 0.f, 0.f, 0.f };
		/// <summary>
		/// Scale of the Object, default to 1.
		/// </summary>
		Math::Vec3f Scale{ 1.f, 1.f, 1.f };
		/// <summary>
		/// Rotation of the Object, default to 0.
		/// </summary>
		Math::Quaternion Rotation{ 0.f, 0.f, 0.f };

		Transform() noexcept = default;
		Transform(const Math::Vec3f& Position) noexcept;
		Transform(const Math::Vec3f& Position, const Math::Vec3f& Scale) noexcept;
		Transform(const Math::Vec3f& Position, const Math::Quaternion& Rotation) noexcept;
		Transform(const Math::Vec3f& Position, const Math::Vec3f& Scale, const Math::Quaternion& Rotation) noexcept;

		/// <summary>
		/// Rotate around the X axis;
		/// </summary>
		/// <param name="x">The angle of roration in degrees</param>
		void RotateX(const float x) noexcept;
		/// <summary>
		/// Rotate around the Y axis;
		/// </summary>
		/// <param name="y">The angle of roration in degrees</param>
		void RotateY(const float y) noexcept;
		/// <summary>
		/// Rotate around the Z axis;
		/// </summary>
		/// <param name="z">The angle of roration in degrees</param>
		void RotateZ(const float z) noexcept;
		/// <summary>
		/// Rotate around the x, z, y axis in that specific order.
		/// </summary>
		/// <param name="xAngle">The rotation to apply to X axis in degrees</param>
		/// <param name="yAngle">The rotation to apply to Y axis in degrees</param>
		/// <param name="zAngle">The rotation to apply to Z axis in degrees</param>
		/// <returns></returns>
		void Rotate(const float xAngle, const float yAngle, const float zAngle) noexcept;

		/// <summary>
		/// Return the forward pointing vector.
		/// </summary>
		Math::Vec3f Forward() const noexcept;
		/// <summary>
		/// Return the right pointing vector.
		/// </summary>
		Math::Vec3f Right() const noexcept;
		/// <summary>
		/// Return the vector pointing upward.
		/// </summary>
		Math::Vec3f Up() const noexcept;
		/// <summary>
		/// Get the transform rotation as euleur angles in degrees.
		/// </summary>
		Math::Vec3f EulerRotation() const noexcept;

		/// <summary>
		/// Implict conversion operator for PhysX Transform implementation.
		/// \warning Does not represent the scale.
		/// </summary>
		/// <returns>A valid PhysX transform representing position, scale and rotation</returns>
		operator physx::PxTransform() const noexcept;
		/// <summary>
		/// Implict conversion operator for DirectXMath matrix implementation.
		/// </summary>
		/// <returns>A valid DirectXMath matrix representing position, scale and rotation</returns>
		operator DirectX::XMMATRIX() const noexcept;

		/// <summary>
		/// Allow copy assignation from physx transform representation.
		/// </summary>
		void operator=(const physx::PxTransform& PhysxTransform) noexcept;

		bool operator==(const Transform& Transform) const noexcept;
		bool operator!=(const Transform& Transform) const noexcept;
	};
}
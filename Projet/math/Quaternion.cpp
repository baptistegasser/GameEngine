#include "stdafx.h"
#include "Quaternion.h"

#include "Math.h"

namespace Math
{
	Quaternion::Quaternion() noexcept : PxQuat(1.f) {}

	Quaternion::Quaternion(const float a, const Vec3f& Axis) noexcept
		: physx::PxQuat{ a, Axis }
	{}

	Quaternion::Quaternion(const float x, const float y, const float z, const float w) noexcept
		: physx::PxQuat{ x, y, z, w }
	{}

	Quaternion::Quaternion(const float pitch, const float yaw, const float roll) noexcept
	{
		auto QuatX = PxQuat(Deg2Rad(pitch), { 1.f, 0.f, 0.f });
		auto QuatY = PxQuat(Deg2Rad(yaw), { 0.f, 1.f, 0.f });
		auto QuatZ = PxQuat(Deg2Rad(roll), { 0.f, 0.f, 1.f });

		*this = (QuatX * QuatY * QuatZ);
	}

	Vec3f Quaternion::ToEuler() const noexcept
	{
		// using https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
		Vec3f EulerAngles{};

		// roll (x-axis rotation)
		float sinr_cosp = 2.f * (w * x + y * z);
		float cosr_cosp = 1.f - 2.f * (x * x + y * y);
		EulerAngles.y = std::atan2f(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		float sinp = 2.f * (w * y - z * x);
		if (std::abs(sinp) >= 1)
			EulerAngles.y = std::copysignf(MATH_PI / 2.f, sinp);
		else
			EulerAngles.y = std::asinf(sinp);

		// yaw (z-axis rotation)
		float siny_cosp = 2.f * (w * z + x * y);
		float cosy_cosp = 1.f - 2.f * (y * y + z * z);
		EulerAngles.z = std::atan2f(siny_cosp, cosy_cosp);

		return Vec3f{ Rad2Deg(EulerAngles.x), Rad2Deg(EulerAngles.y), Rad2Deg(EulerAngles.z) };
	}

	DirectX::XMMATRIX Quaternion::ToXMMatrix() const noexcept
	{
		return DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(x, y, z, w));
	}

	void Quaternion::operator=(const physx::PxQuat& Quaternion) noexcept
	{
		physx::PxQuat::operator=(Quaternion);
	}
}
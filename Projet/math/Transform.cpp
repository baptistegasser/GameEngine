#include "stdafx.h"
#include "Transform.h"

namespace Math
{
	Transform::Transform(const Math::Vec3f& Position) noexcept
		: Position{ Position }
	{}

	Transform::Transform(const Math::Vec3f& Position, const Math::Vec3f& Scale) noexcept
		: Position{ Position }
		, Scale{ Scale }
	{}

	Transform::Transform(const Math::Vec3f& Position, const Math::Quaternion& Rotation) noexcept
		: Position{ Position }
		, Rotation{ Rotation }
	{}

	void Transform::RotateX(const float x) noexcept
	{
		Rotate(x, 0.f, 0.f);
	}

	void Transform::RotateY(const float y) noexcept
	{
		Rotate(0.f, y, 0.f);
	}

	void Transform::RotateZ(const float z) noexcept
	{
		Rotate(0.f, 0.f, z);
	}

	void Transform::Rotate(const float xAngle, const float yAngle, const float zAngle) noexcept
	{
		Rotation *= Math::Quaternion(xAngle, yAngle, zAngle);
	}

	Math::Vec3f Transform::Forward() const noexcept
	{
		const static Math::Vec3f DEFAULT_FORWARD{ 1.0f, 0.f, 0.f };
		return Rotation.rotate(DEFAULT_FORWARD);
	}

	Math::Vec3f Transform::Right() const noexcept
	{
		const static Math::Vec3f DEFAULT_RIGHT{ 0.0f, 0.f, 1.f };
		return Rotation.rotate(DEFAULT_RIGHT);
	}

	Math::Vec3f Transform::Up() const noexcept
	{
		const static Math::Vec3f DEFAULT_UP{ 0.0f, 1.f, 0.f };
		return Rotation.rotate(DEFAULT_UP);
	}

	Math::Vec3f Transform::EulerRotation() const noexcept
	{
		return Rotation.ToEuler();
	}

	Transform::operator physx::PxTransform() const noexcept
	{
		return physx::PxTransform{ Position, Rotation };
	}

	Transform::operator DirectX::XMMATRIX() const noexcept
	{
		using namespace DirectX;
		using namespace Math;

		XMMATRIX ScaleMatrix = XMMatrixScalingFromVector(Scale.ToXMVector());
		XMMATRIX TranslationMatrix = XMMatrixTranslationFromVector(Position.ToXMVector());
		XMMATRIX RotationMatrix = Rotation.ToXMMatrix();

		return ScaleMatrix * RotationMatrix * TranslationMatrix;
	}

	void Transform::operator=(const physx::PxTransform& PhysxTransform) noexcept
	{
		this->Position = PhysxTransform.p;
		this->Rotation = PhysxTransform.q;
	}

	bool Transform::operator==(const Transform& Transform) const noexcept
	{
		return this->Position == Transform.Position
			&& this->Scale == Transform.Scale
			&& this->Rotation == Transform.Rotation;
	}

	bool Transform::operator!=(const Transform& Transform) const noexcept
	{
		return !(*this == Transform);
	}
}
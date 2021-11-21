#include "stdafx.h"
#include "Math.h"

namespace Math
{
	DirectX::XMVECTOR PX2XMVector(const physx::PxVec3& Vec) noexcept
	{
		return DirectX::XMVECTOR{ Vec.x, Vec.y, Vec.z };
	}

	DirectX::XMMATRIX TransformToMatrix(const physx::PxTransform& Transform) noexcept
	{
		using namespace DirectX;
		XMMATRIX Scale = XMMatrixScalingFromVector({ 1.0f, 1.f, 1.f });
		XMMATRIX Translation = XMMatrixTranslationFromVector({ Transform.p.x, Transform.p.y, Transform.p.z });
		XMMATRIX Rotation = XMMatrixRotationZ(0);

		return Scale * Rotation * Translation;
	}
} // namespace Math
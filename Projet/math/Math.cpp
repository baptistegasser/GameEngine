#include "stdafx.h"
#include "Math.h"

namespace Math
{
	DirectX::XMVECTOR PX2XMVector(const physx::PxVec3& Vec) noexcept
	{
		return DirectX::XMVECTOR{ Vec.x, Vec.y, Vec.z };
	}

	physx::PxVec3 XMVector2PX(const DirectX::XMVECTOR& Vec) noexcept
	{
		return physx::PxVec3{DirectX::XMVectorGetX(Vec), DirectX::XMVectorGetY(Vec), DirectX::XMVectorGetZ(Vec)};
	}

	physx::PxQuat XMVector2Quat(const DirectX::XMVECTOR& Vec) noexcept
	{
		return physx::PxQuat{ DirectX::XMVectorGetX(Vec), DirectX::XMVectorGetY(Vec), DirectX::XMVectorGetZ(Vec), 1.0f };
	}

	DirectX::XMMATRIX TransformToMatrix(const physx::PxTransform& Transform) noexcept
	{
		using namespace DirectX;
		XMMATRIX Scale = XMMatrixScalingFromVector({ 1.0f, 1.f, 1.f });
		XMMATRIX Translation = XMMatrixTranslationFromVector({ Transform.p.x, Transform.p.y, Transform.p.z });
		XMMATRIX Rotation = XMMatrixRotationQuaternion(XMVectorSet(Transform.q.x, Transform.q.y, Transform.q.z, Transform.q.w));

		return Scale * Rotation * Translation;
	}
} // namespace Math
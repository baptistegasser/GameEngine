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

	DirectX::XMMATRIX TransformToMatrix(const Transform& Transform) noexcept
	{
		using namespace DirectX;
		XMMATRIX Scale = XMMatrixScalingFromVector(PX2XMVector(Transform.Scale));
		XMMATRIX Translation = XMMatrixTranslationFromVector(PX2XMVector(Transform.PosRot.p));
		XMMATRIX Rotation = XMMatrixRotationQuaternion(XMVectorSet(Transform.PosRot.q.x, Transform.PosRot.q.y, Transform.PosRot.q.z, Transform.PosRot.q.w));

		return Scale * Rotation * Translation;
	}
} // namespace Math
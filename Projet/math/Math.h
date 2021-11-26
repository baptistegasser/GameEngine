#pragma once

#include "PxPhysicsAPI.h"
#include <DirectXMath.h>
#include "../core/Transform.h"

namespace Math
{
	DirectX::XMVECTOR PX2XMVector(const physx::PxVec3& Vec) noexcept;

	physx::PxVec3 XMVector2PX (const DirectX::XMVECTOR& Vec) noexcept;

	physx::PxQuat XMVector2Quat(const DirectX::XMVECTOR& Vec) noexcept;

	DirectX::XMMATRIX TransformToMatrix(const Transform& Transform) noexcept;
} // namespace Math
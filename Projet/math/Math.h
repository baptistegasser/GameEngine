#pragma once

#include "PxPhysicsAPI.h"
#include <DirectXMath.h>

namespace Math
{
	DirectX::XMVECTOR PX2XMVector(const physx::PxVec3& Vec) noexcept;

	DirectX::XMMATRIX TransformToMatrix(const physx::PxTransform& Transform) noexcept;
} // namespace Math
#include "stdafx.h"
#include "Transform.h" 

Transform::Transform(physx::PxTransform PosRot, physx::PxVec3 Scale)
	: PosRot{ PosRot }
	, Scale{ Scale }
{
}

Transform::Transform()
	: PosRot{ physx::PxTransform(0.f, 0.f, 0.f) }
	, Scale{ physx::PxVec3(1.f, 1.f, 1.f) }
{
}

#pragma once

class Transform {
public :
	physx::PxTransform PosRot;
	physx::PxVec3 Scale;

	Transform();
	Transform(physx::PxTransform PosRot, physx::PxVec3 Scale);
};
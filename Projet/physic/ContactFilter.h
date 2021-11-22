#pragma once

#include "PxPhysicsAPI.h"

class ContactFilter {
public:
    static physx::PxFilterFlags filterShader(
        physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
        physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
        physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);

    static void setupFilter(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask);
};

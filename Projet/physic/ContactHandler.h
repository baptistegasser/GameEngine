#pragma once

#include "PxPhysicsAPI.h"

class ContactHandler : public physx::PxSimulationEventCallback, public physx::PxContactModifyCallback {

    void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {}
    virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {}
    virtual void onWake(physx::PxActor** actors, physx::PxU32 count) {}
    virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) {}
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {}
    virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {}
    void onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) override {}
};

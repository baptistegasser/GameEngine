#pragma once

#include "PxPhysicsAPI.h"
#include "Collider.h"
#include <unordered_map>

class ContactHandler : public physx::PxSimulationEventCallback, public physx::PxContactModifyCallback {
public:
    void ClearRegistredCollider(const physx::PxShape* Shape) noexcept;
    // Register the Collider component associated with a physx shape
    void RegisterCollider(const physx::PxShape* Shape, const Collider* Collider) noexcept;

    void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {}
    virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {}
    virtual void onWake(physx::PxActor** actors, physx::PxU32 count) {}
    virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) {}
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
    virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {}
    void onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) override;

private:
    std::unordered_map<const physx::PxShape*, std::vector<const Collider*>> RegisteredColliders;

    void NotifyPairCollider(physx::PxContactModifyPair& ContactPair) noexcept;
    void NotifyPairColliderTrigger(physx::PxTriggerPair& ContactPair) noexcept;
};

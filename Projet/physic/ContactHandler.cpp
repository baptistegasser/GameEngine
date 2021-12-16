#include "stdafx.h"
#include "ContactHandler.h"

#include <functional>

void ContactHandler::ClearRegistredCollider(const physx::PxShape* Shape) noexcept
{
	RegisteredColliders[Shape] = nullptr;
}

void ContactHandler::RegisterCollider(const physx::PxShape* Shape, const Collider* Collider) noexcept
{
	RegisteredColliders[Shape] = Collider;
}

void ContactHandler::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	std::for_each(pairs, pairs + count, std::bind(&ContactHandler::NotifyPairColliderTrigger, this, std::placeholders::_1));
}

void ContactHandler::onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count)
{
	std::for_each(pairs, pairs + count, std::bind(&ContactHandler::NotifyPairCollider, this, std::placeholders::_1));
}

void ContactHandler::NotifyPairCollider(physx::PxContactModifyPair& ContactPair) noexcept
{
    const Collider* FirstCollider = RegisteredColliders[ContactPair.shape[0]];
    const Collider* SecondCollider = RegisteredColliders[ContactPair.shape[1]];

    const auto FirstTrans = FirstCollider->ParentActor->Transform;
    const auto SecondTrans = SecondCollider->ParentActor->Transform;

    FirstCollider->OnContact(Contact(FirstCollider->ParentActor, SecondCollider->ParentActor));
    SecondCollider->OnContact(Contact(SecondCollider->ParentActor, FirstCollider->ParentActor));

    if (FirstTrans != FirstCollider->ParentActor->Transform) {
        // Remove const as we are allowed to modify in this callback
        auto FirstActor = const_cast<physx::PxRigidActor*>(ContactPair.actor[0]);
        FirstActor->setGlobalPose(FirstCollider->ParentActor->Transform);
    }
    if (SecondTrans != SecondCollider->ParentActor->Transform) {
        // Remove const as we are allowed to modify in this callback
        auto SecondActor = const_cast<physx::PxRigidActor*>(ContactPair.actor[1]);
        SecondActor->setGlobalPose(SecondCollider->ParentActor->Transform);
    }
}

void ContactHandler::NotifyPairColliderTrigger(physx::PxTriggerPair& ContactPair) noexcept
{
    const Collider* FirstCollider = RegisteredColliders[ContactPair.triggerShape];
    const Collider* SecondCollider = RegisteredColliders[ContactPair.otherShape];

    const auto FirstTrans = FirstCollider->ParentActor->Transform;
    const auto SecondTrans = SecondCollider->ParentActor->Transform;

    FirstCollider->OnContact(Contact(FirstCollider->ParentActor, SecondCollider->ParentActor));
    SecondCollider->OnContact(Contact(SecondCollider->ParentActor, FirstCollider->ParentActor));

    if (FirstTrans != FirstCollider->ParentActor->Transform) {
        ContactPair.triggerActor->setGlobalPose(FirstCollider->ParentActor->Transform);
    }
    if (SecondTrans != SecondCollider->ParentActor->Transform) {
        ContactPair.otherActor->setGlobalPose(SecondCollider->ParentActor->Transform);
    }
}

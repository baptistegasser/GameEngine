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
	FirstCollider->OnContact(Contact(FirstCollider->ParentActor, SecondCollider->ParentActor));
	SecondCollider->OnContact(Contact(SecondCollider->ParentActor, FirstCollider->ParentActor));
}

void ContactHandler::NotifyPairColliderTrigger(physx::PxTriggerPair& ContactPair) noexcept
{
	const Collider* FirstCollider = RegisteredColliders[ContactPair.triggerShape];
	const Collider* SecondCollider = RegisteredColliders[ContactPair.otherShape];
	FirstCollider->OnContact(Contact(FirstCollider->ParentActor, SecondCollider->ParentActor));
	SecondCollider->OnContact(Contact(SecondCollider->ParentActor, FirstCollider->ParentActor));
}

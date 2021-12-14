#include "stdafx.h"
#include "ContactHandler.h"

#include <functional>

void ContactHandler::ClearRegistredCollider(const physx::PxShape* Shape) noexcept
{
	RegisteredColliders[Shape].clear();
}

void ContactHandler::RegisterCollider(const physx::PxShape* Shape, const Collider* Collider) noexcept
{
	RegisteredColliders[Shape].push_back(Collider);
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
	for (int i = 0; i <= 1; ++i) {
		auto Colliders = RegisteredColliders[ContactPair.shape[i]];
		for (auto& Collider : Colliders) {
			Collider->OnContact({});
		}
	}
}

void ContactHandler::NotifyPairColliderTrigger(physx::PxTriggerPair& ContactPair) noexcept
{
	for (int i = 0; i <= 1; ++i) {
		auto Colliders = RegisteredColliders[ContactPair.triggerShape];
		for (auto& Collider : Colliders) {
			Collider->OnContact({});
		}
	}
}

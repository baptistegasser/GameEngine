#include "stdafx.h"
#include "Actor.h"

#include "physic/RigidBody.h"

namespace Pitbull
{
	Actor::Actor()
		: Name{ "Actor_" + std::to_string(NextID) }
		, ID{ ++NextID }
		, Transform{}
{}

	Actor::Actor(std::string name)
		: Name{std::move(name)}
		, ID{ ++NextID }
		, Transform{}
	{}

	void Actor::Init()
	{
		// Enforce presence of a rigidbody, if none found default to a static body
		const auto RB = GetComponent<RigidBody>();
		if (!RB) {
			AddComponent<RigidBody>(RigidBody::RigidActorType::Static);
		}

		for (const auto& Comp : Components) {
			Comp->Init();
		}
	}

	void Actor::Tick(const float ElapsedTime)
	{
		if (!Active) return;
		for (const auto& Comp : Components) {
			Comp->Tick(ElapsedTime);
		}
	}

	void Actor::FixedTick(const float DeltaTime)
	{
		if (!Active) return;
		for (const auto& Comp : Components) {
			Comp->FixedTick(DeltaTime);
		}
	}

	void Actor::LateTick(const float ElapsedTime)
	{
		if (!Active) return;
		for (const auto& Comp : Components) {
			Comp->LateTick(ElapsedTime);
		}
	}

	void Actor::UITick(const float ElapsedTime)
	{
		if (!Active) return;
		for (const auto& Comp : Components) {
			Comp->UITick(ElapsedTime);
		}
	}

	void Actor::SpriteTick(const float ElapsedTime)
	{
		if (!Active) return;
		for (const auto& Comp : Components) {
			Comp->SpriteTick(ElapsedTime);
		}
	}

	Actor::ComponentList Actor::GetComponents() const
	{
		ComponentList All;
		All.reserve(Components.size());

		for (auto& Comp : Components)
			All.push_back(Comp.get());

		return All;
	}

	Actor::ActorID Actor::NextID = 0;
} // namespace Pitbull

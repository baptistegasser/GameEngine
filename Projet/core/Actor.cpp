#include "stdafx.h"
#include "Actor.h"

#include "Component.h"

namespace Pitbull
{
	Actor::Actor()
		: ID{ NextID++ }
		, Name{ "Actor_" + std::to_string(NextID-1) }
	{}

	Actor::Actor(std::string name)
		: ID{ NextID++ }
		, Name{std::move(name)}
	{}

	Actor::~Actor()
	{
		// Delete all our components
		for (const auto Comp : Components) {
			delete Comp;
		}
	}

	void Actor::Init()
	{
		for (const auto Comp : Components) {
			Comp->Init();
		}
	}

	void Actor::Tick()
	{
		for (auto& Comp : Components)
		{
			Comp->Tick(0.f /* TODO deltatime */);
		}
	}

	Actor::ActorID Actor::NextID = 0;

	void Actor::AddComponent(Component* Comp)
	{
		Comp->SetParentActor(this);
		Components.push_back(Comp);
	}

	template <class Impl>
	Impl* Actor::GetComponent()
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		for (auto Comp : Components) {
			auto CompImpl = dynamic_cast<Impl*>(Comp);
			if (CompImpl) {
				return CompImpl;
			}
		}

		throw std::logic_error{ std::string{"The actor has no component of type "} + typeid(Impl).name() };
	}

	template <class Impl>
	std::vector<Impl*> Actor::GetComponents()
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		std::vector<Impl*> MatchingComponents;

		for (auto Comp : Components) {
			auto CompImpl = dynamic_cast<Impl*>(Comp);
			if (CompImpl) {
				MatchingComponents.push_back(CompImpl);
			}
		}

		return MatchingComponents;
	}
} // namespace Pitbull

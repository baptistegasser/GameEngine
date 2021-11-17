#include "stdafx.h"
#include "Actor.h"

#include "Component.h"

namespace Pitbull
{
	Actor::Actor()
		: Name{ "Actor_" + std::to_string(NextID) }
		, ID{ ++NextID }
	{}

	Actor::Actor(std::string name)
		: Name{std::move(name)}
		, ID{ ++NextID }
	{}

	void Actor::Init()
	{
		for (const auto& Comp : Components) {
			Comp->Init();
		}
	}

	void Actor::Tick()
	{
		for (const auto& Comp : Components)
		{
			Comp->Tick(0.f /* TODO deltatime */);
		}
	}

	Actor::ActorID Actor::NextID = 0;

	template <class Impl, class ... Args>
	void Actor::AddComponent(Args&&... Args)
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		Components.push_back(std::make_unique<Component>(this, new Impl{ std::forward(Args ...) }));
	}

	template <class Impl>
	Impl* Actor::GetComponent() const
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		for (auto Comp : Components) {
			auto CompImpl = dynamic_cast<Impl*>(Comp.get());
			if (CompImpl) {
				return CompImpl;
			}
		}

		throw std::logic_error{ std::string{"The actor has no component of type "} + typeid(Impl).name() };
	}

	template <class Impl>
	std::vector<Impl*> Actor::GetComponents() const
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		std::vector<Impl*> MatchingComponents;

		for (auto Comp : Components) {
			auto CompImpl = dynamic_cast<Impl*>(Comp.get());
			if (CompImpl) {
				MatchingComponents.push_back(CompImpl);
			}
		}

		return MatchingComponents;
	}
} // namespace Pitbull

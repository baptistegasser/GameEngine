#pragma once

#include "math/Transform.h"

#include <string>
#include <vector>

namespace Pitbull
{
	// Replace include with incomplete type to prevent cyclic dependencies
	class IComponent;
	template <class Impl>
	class Component;

	using ActorID = unsigned int;

	/// <summary>
	/// Represent anything that can be placed on the scene of a game.
	///	The actor logic is defined by the composition of its components.
	/// </summary>
	class Actor {
	public:
		explicit Actor();
		explicit Actor(std::string name);
		~Actor();

		template <class Impl>
		void AddComponent(Component<Impl>* Comp);

		template <class Impl>
		Component<Impl>& GetComponent();
		template <class Impl>
		std::vector<std::reference_wrapper<Component<Impl>>> GetComponents();

	public:
		const ActorID ID;
		const std::string Name;
		math::Transform Transform;

	private:
		static ActorID NextID;
		std::vector<IComponent*> Components;
	};

	template <class Impl>
	void Actor::AddComponent(Component<Impl>* Comp)
	{
		Comp->SetParentActor(this);
		Components.push_back(Comp);
	}

	template <class Impl>
	Component<Impl>& Actor::GetComponent()
	{
		static_assert(std::is_base_of<Component<Impl>, Impl>::value, "The passed type is not a Component.");
		const auto CompID = Impl::GetIDStatic();

		for (auto Comp : Components) {
			if (Comp->GetID() == CompID)
			{
				return *static_cast<Impl*>(Comp);
			}
		}

		throw 1;
	}

	template <class Impl>
	std::vector<std::reference_wrapper<Component<Impl>>> Actor::GetComponents()
	{
		static_assert(std::is_base_of<Component<Impl>, Impl>::value, "The passed type is not a Component.");
		const auto CompID = Impl::GetIDStatic();

		std::vector<std::reference_wrapper<Component<Impl>>> MatchingComponents;

		for (auto Comp : Components) {
			if (Comp->GetID() == CompID)
			{
				MatchingComponents.push_back(*static_cast<Impl*>(Comp));
			}
		}

		return MatchingComponents;
	}
} // namespace Pitbull

#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <memory>

#include "Component.h"

namespace Pitbull
{
	/// <summary>
	/// Represent anything that can be placed on the scene of a game.
	///	The actor logic is defined by the composition of its components.
	/// </summary>
	class Actor {
	public:
		using ActorID = unsigned int;

		explicit Actor();
		explicit Actor(std::string name);
		~Actor() = default;

		void Init();
		void Tick();

		template <class ... Args>
		static std::shared_ptr<Actor> New(Args&&... args);

		template <class Impl, class ... Args>
		void AddComponent(Args&&... args);

		/// <summary>
		/// Get the first component that match a type.
		/// </summary>
		/// <typeparam name="Impl">The desired type.</typeparam>
		/// <returns>A reference to the stored component or fail if none found.</returns>
		///	<example>auto& comp = actor.GetComponent<MyComponent>();</example>
		template <class Impl>
		Impl* GetComponent() const;

		/// <summary>
		/// Get all components that match a type.
		/// </summary>
		/// <typeparam name="Impl">The desired type.</typeparam>
		/// <returns>A vector of references to the stored components, can be empty.</returns>
		///	<example>auto vec = actor.GetComponents<MyComponent>();</example>
		template <class Impl>
		std::vector<Impl*> GetComponents() const;

		std::vector<Component*> GetComponents() const;
		
		const std::string Name;
		physx::PxTransform Transform;
		const ActorID ID;
		static ActorID NextID;

	private:
		std::vector<Component*> Components;

	public:
		int GridX = 0;
		int GridY = 0;
	};

	template<class ...Args>
	inline std::shared_ptr<Actor> Actor::New(Args && ...args)
	{
		return std::make_shared<Actor>(std::forward<Args>(Arguments)...);
	}

	template <class Impl, class ... Args>
	void Actor::AddComponent(Args&&... Arguments)
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		Components.push_back(new Impl{ this, std::forward<Args>(Arguments)... });
	}

	template <class Impl>
	Impl* Actor::GetComponent() const
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		for (auto Comp : Components) {
			auto CompImpl = dynamic_cast<Impl*>(Comp);
			if (CompImpl) {
				return CompImpl;
			}
		}

		//throw std::logic_error{ std::string{"The actor has no component of type "} + typeid(Impl).name() };
		return nullptr;
	}

	template <class Impl>
	std::vector<Impl*> Actor::GetComponents() const
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		std::vector<Impl*> MatchingComponents;

		for (auto& Comp : Components) {
			auto CompImpl = dynamic_cast<Impl*>(Comp);
			if (CompImpl) {
				MatchingComponents.push_back(CompImpl);
			}
		}

		return MatchingComponents;
	}


} // namespace Pitbull

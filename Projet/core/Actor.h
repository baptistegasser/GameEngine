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
		using ComponentPtr = Component*;
		using ComponentList = std::vector<ComponentPtr>;
		template <class Impl> using ImplPtr = Impl*;
		template <class Impl> using ImplList = std::vector<ImplPtr<Impl>>;

		/// <summary>
		/// The unique identifier of an actor.
		/// </summary>
		const ActorID ID;
		/// <summary>
		/// Used to generate unique IDs
		/// </summary>
		static ActorID NextID;
		/// <summary>
		/// The name of this actor, used to identify this actor easily.
		/// </summary>
		const std::string Name;
		physx::PxTransform Transform;

		explicit Actor();
		explicit Actor(std::string name);
		~Actor() = default;

		void Init();
		void Tick(const float ElapsedTime);
		void FixedTick(const float DeltaTime);

		template <class ... Args>
		static std::unique_ptr<Actor> New(Args&&... args);

		/// <summary>
		/// Create and add a component to this actor.
		/// </summary>
		/// <typeparam name="Impl">The class of the component to create</typeparam>
		/// <typeparam name="...Args">Generic variadic args</typeparam>
		/// <param name="...args">Arguments for the component constructors</param>
		/// <returns>A pointer to the newly created component</returns>
		template <class Impl, class ... Args>
		Actor::ImplPtr<Impl> AddComponent(Args&&... args);

		/// <summary>
		/// Get the first component that match a type.
		/// </summary>
		/// <typeparam name="Impl">The desired type.</typeparam>
		/// <returns>A reference to the stored component or fail if none found.</returns>
		///	<example>auto& comp = actor.GetComponent<MyComponent>();</example>
		template <class Impl>

		ImplPtr<Impl> GetComponent() const;
		/// <summary>
		/// Get all components that match a type.
		/// </summary>
		/// <typeparam name="Impl">The desired type.</typeparam>
		/// <returns>A vector of references to the stored components, can be empty.</returns>
		///	<example>auto vec = actor.GetComponents<MyComponent>();</example>
		template <class Impl>
		ImplList<Impl> GetComponents() const;

		/// <summary>
		/// Simply get all components of this actor.
		/// </summary>
		/// <returns></returns>
		ComponentList GetComponents() const;

		/// <summary>
		/// Get Components that match a flag.
		///	See \ref Component::ComponentTypeFlag for possible values
		/// </summary>
		ComponentList GetFlaggedComponents(const Component::ComponentTypeFlag Flag) const;

		/// <summary>
		/// Get Components that don't match a flag.
		///	See \ref Component::ComponentTypeFlag for possible values
		/// </summary>
		ComponentList GetNotFlaggedComponents(const Component::ComponentTypeFlag Flag) const;

	private:
		/// <summary>
		/// The list of component associated to this actor, defining
		///	it's logic and behaviour during runtime;
		/// </summary>
		ComponentList Components;
	};

	template<class ...Args>
	inline std::unique_ptr<Actor> Actor::New(Args && ...args)
	{
		return std::make_unique<Actor>(std::forward<Args>(Arguments)...);
	}

	template <class Impl, class ... Args>
	Actor::ImplPtr<Impl> Actor::AddComponent(Args&&... Arguments)
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");
		
		auto Comp = new Impl{ this, std::forward<Args>(Arguments)... };
		Components.push_back(Comp);
		return Comp;
	}

	template <class Impl>
	Actor::ImplPtr<Impl> Actor::GetComponent() const
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		for (auto Comp : Components) {
			auto CompImpl = dynamic_cast<ImplPtr<Impl>>(Comp);
			if (CompImpl) {
				return CompImpl;
			}
		}

		//throw std::logic_error{ std::string{"The actor has no component of type "} + typeid(Impl).name() };
		return nullptr;
	}

	template <class Impl>
	Actor::ImplList<Impl> Actor::GetComponents() const
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		ImplList<Impl> MatchingComponents;

		for (auto& Comp : Components) {
			auto CompImpl = dynamic_cast<ImplPtr<Impl>>(Comp);
			if (CompImpl) {
				MatchingComponents.push_back(CompImpl);
			}
		}

		return MatchingComponents;
	}


} // namespace Pitbull

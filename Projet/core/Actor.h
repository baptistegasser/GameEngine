#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <memory>

#include "Component.h"
#include "math/Transform.h"

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
		Math::Transform Transform;

		explicit Actor();
		explicit Actor(std::string name);
		virtual ~Actor() = default;

		void Init();
		/// <summary>
		/// Call all components \ref Component::Tick method.
		/// </summary>
		virtual void Tick(const float ElapsedTime);
		/// <summary>
		/// Call all components \ref Component::FixedTick method.
		/// </summary>
		virtual void FixedTick(const float DeltaTime);
		/// <summary>
		/// Call all components \ref Component::LateTick method.
		/// </summary>
		virtual void LateTick(const float ElapsedTime);
		/// <summary>
		/// Call all components \ref Component::SpriteTick method.
		/// </summary>
		virtual void SpriteTick(const float ElapsedTime);

		template <class ... Args>
		static std::unique_ptr<Actor> New(Args&&... Arguments);

		/// <summary>
		/// Create and add a component to this actor.
		/// </summary>
		/// <typeparam name="Impl">The class of the component to create</typeparam>
		/// <typeparam name="...Args">Generic variadic args</typeparam>
		/// <param name="...Arguments">Arguments for the component constructors</param>
		/// <returns>A pointer to the newly created component</returns>
		template <class Impl, class ... Args>
		Impl* AddComponent(Args&&... Arguments);

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

		/// <summary>
		/// Simply get all components of this actor.
		/// </summary>
		/// <returns></returns>
		ComponentList GetComponents() const;

	private:
		/// <summary>
		/// The list of component associated to this actor, defining
		///	it's logic and behaviour during runtime;
		/// </summary>
		std::vector<std::unique_ptr<Component>> Components;
	};

	template<class ...Args>
	std::unique_ptr<Actor> Actor::New(Args && ...Arguments)
	{
		return std::make_unique<Actor>(std::forward<Args>(Arguments)...);
	}

	template <class Impl, class ... Args>
	Impl* Actor::AddComponent(Args&&... Arguments)
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");
		
		auto Comp = new Impl{ this, std::forward<Args>(Arguments)... };
		Components.push_back(std::unique_ptr<Component>{Comp});
		return Comp;
	}

	template <class Impl>
	Impl* Actor::GetComponent() const
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		for (auto& Comp : Components) {
			if (auto CompImpl = dynamic_cast<Impl*>(Comp.get())) {
				return CompImpl;
			}
		}

		return nullptr;
	}

	template <class Impl>
	std::vector<Impl*> Actor::GetComponents() const
	{
		static_assert(std::is_base_of<Component, Impl>::value, "The passed type is not a Component.");

		std::vector<Impl*> MatchingComponents;

		for (auto& Comp : Components) {
			if (auto CompImpl = dynamic_cast<Impl*>(Comp.get())) {
				MatchingComponents.push_back(CompImpl);
			}
		}

		return MatchingComponents;
	}


} // namespace Pitbull

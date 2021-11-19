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
		std::vector<std::unique_ptr<Component>> Components;
	};
} // namespace Pitbull

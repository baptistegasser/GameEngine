#pragma once

#include "Actor.h"

namespace Pitbull
{
	using ComponentID = unsigned int;
	// Static counter to get unique ID for each Component classes
	static ComponentID NextID = 0;

	/// <summary>
/// Definition of a component and it's interface.
///	This should be used to store components, notably pointers.
///	However, us Component<Impl> to create new component classes.
/// </summary>
	class IComponent {
	public:
		// Virtual, prevent direct instantiation of this class.
		virtual ~IComponent() = 0;
		/// <summary>
	/// Get the unique ID associated to a component class.
	///	Each class should have a unique ID, and each instance of
	///	said class must return the same ID.
	/// </summary>
	/// <returns>The ID of the component type.</returns>
		virtual ComponentID GetID() const noexcept = 0;

		/// <summary>
	/// Set the actor that contains this component.
	/// </summary>
		void SetParentActor(Actor* Actor) noexcept;

		/// <summary>
	/// The component logic, runned at a regular interval.
	/// </summary>
	/// <param name="DeltaTime">The elapsed time since last call.</param>
		virtual void Tick(const float& DeltaTime);

	private:
		/// <summary>
	/// The actor that contains this component.
	/// </summary>
		Actor* ParentActor = nullptr;
	};

	/// <summary>
///	Proxy pattern used to create component classes that respect the IComponent
///	class's contract by providing unique IDs.
/// </summary>
/// <typeparam name="Impl">The actual component class created.</typeparam>
///	<example>
///		// Create a new Component to control the player
///		class PlayerController : public Component<PlayerController> {
///		public:
///			~PlayerController() {}
///		}
///	</example>
	template <class Impl>
	class Component : public IComponent {
	public:
		// Virtual, prevent direct instantiation of this class.
		~Component() override = 0;

		/// <summary>
	/// Get the unique ID of this component respecting IComponent contract.
	/// </summary>
		ComponentID GetID() const noexcept override final
		{
			return ID;
		}
		/// <summary>
	/// Allow to get the unique ID of this class without instantiating it.
	/// </summary>
		static ComponentID GetIDStatic() noexcept
		{
			return ID;
		}

	private:
		/// <summary>
	/// The proxied component ID.
	/// </summary>
		static ComponentID ID;
	};

	template <class Impl>
	Component<Impl>::~Component() = default;

	template <class Impl>
	ComponentID Component<Impl>::ID = NextID++;
}  // namespace Pitbull

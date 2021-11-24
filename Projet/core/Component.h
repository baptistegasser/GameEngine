#pragma once

namespace Pitbull
{
	class Actor;

	/// <summary>
	/// Definition of a component and it's interface.
	///	This should be used to store components, notably pointers.
	///	However, us Component<Impl> to create new component classes.
	/// </summary>
	class Component {
	public:
		// Virtual, prevent direct instantiation of this class.
		virtual ~Component() = 0;

		/// <summary>
		/// Set the actor that contains this component.
		/// </summary>
		void SetParentActor(Actor* Actor) noexcept;

		/// <summary>
		/// Method called once when initializing a component.
		/// </summary>
		virtual void Init();

		/// <summary>
		/// The component logic, runned at a regular interval matching the frame rate.
		/// </summary>
		/// <param name="ElapsedTime">The elapsed time since last call.</param>
		virtual void Tick(const float& ElapsedTime);

		/// <summary>
		/// The component logic, runned at a fixed interval matching the physic simulation update.
		/// </summary>
		/// <param name="DeltaTime">The fixed delta time of the physic simulation.</param>
		virtual void FixedTick(const float& DeltaTime);

		/// <summary>
		/// Type are used to flag a components.
		///	The defined flags are only used by the engine to find specific
		///	components relating to a specific subsystem (ie: physic stuff).
		/// </summary>
		enum ComponentTypeFlag {
			DEFAULT_COMPONENT = 1 << 0,
			PHYSIC_COMPONENT  = 1 << 1,
			RENDER_COMPONENT  = 1 << 2,
		};
		/// <summary>
		/// The component flags.
		/// </summary>
		int TypeFlags = DEFAULT_COMPONENT;

	protected:
		/// <summary>
		/// The actor that contains this component.
		/// </summary>
		Actor* ParentActor = nullptr;

		Component(Actor* Parent) : ParentActor{ Parent }{}
	};
}  // namespace Pitbull

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
		/// A late component logic, called after \ref Tick.
		///	Useful for components that want to wait for other components to apply their logic before computing.
		/// </summary>
		/// <param name="ElapsedTime">The elapsed time since last call, same as the one passed to \ref Tick.</param>
		virtual void LateTick(const float& ElapsedTime);

		virtual void UITick(const float& ElapsedTime);

		virtual void SpriteTick(const float& ElapsedTime);

		/// <summary>
		/// The actor that contains this component.
		/// </summary>
		Actor* ParentActor = nullptr;

	protected:

		Component(Actor* Parent) : ParentActor{ Parent }{}
	};
}  // namespace Pitbull

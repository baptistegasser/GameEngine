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
		/// The component logic, runned at a regular interval.
		/// </summary>
		/// <param name="DeltaTime">The elapsed time since last call.</param>
		virtual void Tick(const float& DeltaTime);

	protected:
		/// <summary>
		/// The actor that contains this component.
		/// </summary>
		Actor* ParentActor = nullptr;

		Component(Actor* Parent) : ParentActor{ Parent }{}
	};
}  // namespace Pitbull

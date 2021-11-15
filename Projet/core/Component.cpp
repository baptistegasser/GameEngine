#include "Component.h"

namespace Pitbull
{
	// Default implementation must be provided to destructor, even if pure virtual
	IComponent::~IComponent() = default;

	void IComponent::SetParentActor(Actor* Actor) noexcept
{
	this->ParentActor = Actor;
}

	void IComponent::Tick(const float& /*DeltaTime*/)
	{
		// Default component logic is to do nothing
	}
}  // namespace Pitbull

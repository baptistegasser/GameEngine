#include "stdafx.h"
#include "Component.h"

#include "Actor.h"

namespace Pitbull
{
	// Default implementation must be provided to destructor, even if pure virtual
	Component::~Component() = default;

	void Component::SetParentActor(Actor* Actor) noexcept
	{
		this->ParentActor = Actor;
	}

	void Component::Init()
	{
		// Default init behaviour is to do nothing
	}

	void Component::Tick(const float& /*DeltaTime*/)
	{
		// Default component logic is to do nothing
	}
}  // namespace Pitbull

#include "stdafx.h"
#include "Component.h"

#include "Actor.h"

namespace Pitbull
{
	// Default implementation must be provided to destructor, even if pure virtual
	Component::~Component() = default;

	void Component::Init()
	{
		// Default init behaviour is to do nothing
	}

	void Component::Tick(const float& /*ElapsedTime*/)
	{
		// Default component logic is to do nothing
	}

	void Component::FixedTick(const float& DeltaTime)
	{
		// Default fixed component logic is to do nothing
	}

	void Component::LateTick(const float& ElapsedTime)
	{
		// Default late component logic is to do nothing
	}

	void Component::UITick(const float& ElapsedTime)
	{
		// Default UI component logic is to do nothing
	}

	void Component::SpriteTick(const float& ElapsedTime)
	{
		// Default late component logic is to do nothing
	}
}  // namespace Pitbull

#include "stdafx.h"
#include "Actor.h"

#include <utility>
#include "Component.h"

namespace Pitbull
{
	Actor::Actor()
		: ID{ NextID++ }
		, Name{ "Actor_" + std::to_string(NextID-1) }
	{}

	Actor::Actor(std::string name)
		: ID{ NextID++ }
		, Name{std::move(name)}
	{}

	Actor::~Actor()
	{
		// Delete all our components
		for (const auto Comp : Components) {
			delete Comp;
		}
	}

	ActorID Actor::NextID = 0;
} // namespace Pitbull

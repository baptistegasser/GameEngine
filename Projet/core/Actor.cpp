#include "stdafx.h"
#include "Actor.h"

namespace Pitbull
{
	Actor::Actor()
		: Name{ "Actor_" + std::to_string(NextID) }
		, ID{ ++NextID }
	{}

	Actor::Actor(std::string name)
		: Name{std::move(name)}
		, ID{ ++NextID }
	{}

	void Actor::Init()
	{
		for (const auto& Comp : Components) {
			Comp->Init();
		}
	}

	void Actor::Tick()
	{
		for (const auto& Comp : Components)
		{
			Comp->Tick(0.f /* TODO deltatime */);
		}
	}

	std::vector<Component*> Actor::GetComponents() const
	{
		std::vector<Component*> All;
		All.reserve(Components.size());

		for (auto& Comp : Components)
			All.push_back(Comp);

		return All;
	}

	Actor::ActorID Actor::NextID = 0;
} // namespace Pitbull

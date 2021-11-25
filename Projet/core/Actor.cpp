#include "stdafx.h"
#include "Actor.h"

namespace Pitbull
{
	Actor::Actor()
		: Name{ "Actor_" + std::to_string(NextID) }
		, ID{ ++NextID }
		, Transform{ 0.f, 0.f, 0.f }
	{}

	Actor::Actor(std::string name)
		: Name{std::move(name)}
		, ID{ ++NextID }
		, Transform{ 0.f, 0.f, 0.f }
	{}

	void Actor::Init()
	{
		for (const auto& Comp : Components) {
			Comp->Init();
		}
	}

	void Actor::Tick(const float ElapsedTime)
	{
		for (const auto& Comp : Components) {
			Comp->Tick(ElapsedTime);
		}
	}

	void Actor::FixedTick(const float DeltaTime)
	{
		for (const auto& Comp : Components) {
			Comp->FixedTick(DeltaTime);
		}
	}

	void Actor::LateTick(const float ElapsedTime)
	{
		for (const auto& Comp : Components) {
			Comp->LateTick(ElapsedTime);
		}
	}

	Actor::ComponentList Actor::GetComponents() const
	{
		ComponentList All;
		All.reserve(Components.size());

		for (auto& Comp : Components)
			All.push_back(Comp.get());

		return All;
	}

	Actor::ActorID Actor::NextID = 0;
} // namespace Pitbull

#include "stdafx.h"
#include "Timer.h"

#include "EngineD3D11.h"
#include "core/Actor.h"

Timer::Timer(Pitbull::Actor* Parent)
	: Component{Parent}
{
}

void Timer::Init()
{
	// Get the needed components only once at init
	MySprite = ParentActor->GetComponent<TextRenderer>();
	Start = std::chrono::steady_clock::now();
	Last = Start;
}

void Timer::FixedTick(const float& DeltaTime)
{
	Now = std::chrono::steady_clock::now();
	if (!EngineD3D11::GetInstance().IsPaused())
	{
		Duration = std::chrono::duration_cast<std::chrono::seconds>(Now - Start - IgnoreDuration);
		const auto Seconds = Duration.count();
		MySprite->Write(L"Time : " + std::to_wstring(Seconds /60) + L" min " + std::to_wstring(Seconds % 60) + L" s");
	}
	else
	{
		IgnoreDuration += std::chrono::duration_cast<std::chrono::nanoseconds>(Now - Last);
	}
	Last = Now;
}

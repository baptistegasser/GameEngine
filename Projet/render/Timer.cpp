#include "stdafx.h"
#include "Timer.h"
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
}

void Timer::FixedTick(const float& DeltaTime)
{
	End = std::chrono::steady_clock::now();
	Duration = std::chrono::duration_cast<std::chrono::seconds>(End - Start).count();
	MySprite->Write(L"Time : " + std::to_wstring(Duration /60) + L" min " + std::to_wstring(Duration % 60) + L" s");
}

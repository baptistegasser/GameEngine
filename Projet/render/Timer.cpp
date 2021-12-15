#include "stdafx.h"
#include "Timer.h"
#include "render/MoteurWindows.h"

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
	MySprite->Write(L"Time : " + std::to_wstring(std::chrono::duration_cast<std::chrono::milliseconds>(End - Start).count()) + L" ms");
}

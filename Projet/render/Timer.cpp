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
	MyTimer = ParentActor->GetComponent<SpriteRenderer>();
	Start = std::chrono::steady_clock::now();
}

void Timer::FixedTick(const float& DeltaTime)
{
	End = std::chrono::steady_clock::now();
	MyTimer->Write(L"Time : " + std::to_wstring(std::chrono::duration_cast<std::chrono::milliseconds>(End - Start).count()) + L" ms");
}

#pragma once
#include "core/Component.h"
#include "render/TextRenderer.h"
#include <chrono>

class Timer : public Pitbull::Component {

public :
	Timer(Pitbull::Actor* Parent);
	~Timer() override = default;

	void Init() override;

	/// <summary>
	/// Update the time of the textrenderer
	/// </summary>
	void FixedTick(const float& DeltaTime) override;

	std::wstring GetValue();
private :
	TextRenderer* MySprite;
	std::chrono::steady_clock::time_point Start;
	std::chrono::steady_clock::time_point Last;
	std::chrono::steady_clock::time_point Now;
	std::chrono::nanoseconds IgnoreDuration;
	std::chrono::seconds Duration;
};
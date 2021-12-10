#pragma once
#include "core/Component.h"
#include "render/SpriteRenderer.h"
#include <chrono>


class Timer : public Pitbull::Component {

public :
	Timer(Pitbull::Actor* Parent);
	~Timer() override = default;

	void Init() override;

	/// <summary>
	/// Update the spriterenderer
	/// </summary>
	void FixedTick(const float& DeltaTime) override;
private :
	SpriteRenderer* MyTimer;
	std::chrono::steady_clock::time_point Start;
	std::chrono::steady_clock::time_point End;
};
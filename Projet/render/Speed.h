#pragma once
#include "core/Component.h"
#include "render/TextRenderer.h"
#include "physic/RigidBody.h"
#include <chrono>


class Speed : public Pitbull::Component {

public :
	Speed(Pitbull::Actor* Parent);
	~Speed() override = default;

	void Init() override;

	/// <summary>
	/// Update the velocity of the textrenderer
	/// </summary>
	void FixedTick(const float& DeltaTime) override;

private :
	RigidBody* MyRigidBody;
	TextRenderer* MySprite;
};
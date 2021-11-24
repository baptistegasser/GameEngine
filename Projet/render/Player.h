#pragma once

#include "core/Component.h"

class Player : public Pitbull::Component
{
public :
	Player(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Direction);
public:
	~Player() = default;

	/// <summary>
	/// Update the direction of the player
	/// Update the actions (bool) the player want to do
	/// Update the type of camera
	/// </summary>
	/// <param name="DeltaTime"></param>
	void Tick(const float& DeltaTime) override;

	/// <summary>
	/// Type of camera, first or third person
	/// </summary>
	enum CAMERA_TYPE {
		FIRST,
		THIRD
	};

	CAMERA_TYPE type;

	bool Forward;
	bool Backward;
	bool Left;
	bool Right;
	bool Jump;

	DirectX::XMVECTOR Direction;
	DirectX::XMVECTOR RelativeZ;

	float Speed = 0.3f;
	float JumpSpeed = 2.0f;

private:
	float AngleRotation = 0.0f;
	float RotationSpeed = 0.05f;

	/// <summary>
	/// Swap the camera's type
	/// </summary>
	void SwapCameraMode();

	bool WaitForSwap = false;

};


#pragma once

#include "core/Component.h"
#include "physic/RigidBody.h"
#include "render/Camera.h"

class Player : public Pitbull::Component
{
public :
	Player(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Direction);
	~Player() override = default;

	void Init() override;

	/// <summary>
	/// Update the direction of the player
	/// Update the type of camera
	/// </summary>
	void FixedTick(const float& DeltaTime) override;

	/// <summary>
	/// Type of camera, first or third person
	/// </summary>
	enum class CameraViewType {
		First,
		Third
	};

private:
	RigidBody* MyRigidBody;
	Camera* MyCamera;

	float AngleRotation = 0.0f;
	float RotationSpeed = 0.05f;

	CameraViewType ViewType;

	DirectX::XMVECTOR Direction;
	DirectX::XMVECTOR RelativeZ;

	float Speed = 100.0f;
	float JumpSpeed = 10.f;

	/// <summary>
	/// Swap the camera's type
	/// </summary>
	void SwapCameraMode();

	bool WaitForSwap = false;
};


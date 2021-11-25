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
	void Tick(const float& DeltaTime) override;

	/// <summary>
	/// Type of camera, first or third person
	/// </summary>
	enum class CameraViewType {
		First,
		Third
	};

	CameraViewType ViewType;

	DirectX::XMVECTOR Direction;
	DirectX::XMVECTOR RelativeZ;

	float Speed = 0.03f;
	float JumpSpeed = 0.5f;

private:
	RigidBody* MyRigidBody;
	Camera* MyCamera;

	float AngleRotation = 0.0f;
	float RotationSpeed = 0.05f;

	/// <summary>
	/// Swap the camera's type
	/// </summary>
	void SwapCameraMode();

	bool WaitForSwap = false;
};


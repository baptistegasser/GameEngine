#pragma once

#include "core/Component.h"
#include "physic/RigidBody.h"
#include "physic/SphereCollider.h"
#include "render/Camera.h"

class Player : public Pitbull::Component
{
public :
	Player(Pitbull::Actor* Parent);
	~Player() override = default;

	void Init() override;

	/// <summary>
	/// Update the direction of the player
	/// Update the type of camera
	/// </summary>
	void FixedTick(const float& DeltaTime) override;

	bool isGrounded() const;

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
	SphereCollider* MyCollider;

	/****
	 *
	 * VERTICAL CAMERA ANGLE
	 *
	 *****/
	const float MIN_ANGLE_VERTICAL = 0.f;
	const float MAX_ANGLE_VERTICAL = 20.f;
	float AngleRotationVertical = 10.0f;
	const float AngleRotationSpeedVertical = 0.5f;

	/****
	 *
	 * SENSIBILITY OF HORIZONTAL CAMERA
	 *
	 *****/
	const int MIN_SENSIBILITY_HORIZONTAL = 0;
	const int MAX_SENSIBILITY_HORIZONTAL = 5;
	int SensibilityHorizontal = 2;

	/****
	 *
	 * SENSIBILITY OF VERTICAL CAMERA
	 *
	 *****/
	const int MIN_SENSIBILITY_VERTICAL = 0;
	const int MAX_SENSIBILITY_VERTICAL = 5;
	int SensibilityVertical = 3;

	CameraViewType ViewType;

	DirectX::XMVECTOR Direction;
	DirectX::XMVECTOR RelativeZ;

	float Speed = 110.0f;
	float JumpSpeed = 30.f;
	float MaxSpeed = 15.0f;

	/// <summary>
	/// Swap the camera's type
	/// </summary>
	void SwapCameraMode();

	bool WaitForSwap = false;
};


#pragma once

#include "core/Component.h"
#include "physic/RigidBody.h"
#include "physic/SphereCollider.h"
#include "render/Camera.h"

class Player : public Pitbull::Component
{
public :
	Player(Pitbull::Actor* Parent, Math::Vec3f Pos);
	~Player() override = default;

	void Init() override;

	/// <summary>
	/// Update the death state
	/// </summary>
	void Tick(const float& ElapsedTime) override;

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

	void SetSpawnPos(const Math::Vec3f Pos)
	{
		SpawnPos = Pos;
	}

private:
	RigidBody* MyRigidBody;
	Camera* MyCamera;
	SphereCollider* MyCollider;

	Math::Vec3f SpawnPos = Math::Vec3f{ 0.0f, 0.0f, 0.0f };

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

	float Speed = 100.0f;
	float JumpSpeed = 10.f;

	/// <summary>
	/// Swap the camera's type
	/// </summary>
	void SwapCameraMode();

	/// <summary>
	/// Check if player is dead
	/// </summary>
	[[nodiscard]] bool IsDead() const;

	/// <summary>
	/// Reset player value
	/// </summary>
	void ResetPlayer() const;

	bool WaitForSwap = false;
};


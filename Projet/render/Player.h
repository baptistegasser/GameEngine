#pragma once

#include "core/Component.h"

class Player : public Pitbull::Component
{
public :
	Player(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Direction, const DirectX::XMVECTOR& UpDirection);
public:
	~Player() = default;

	void Tick(const float& DeltaTime) override;

	void SetPosition(const DirectX::XMVECTOR & NewPosition) noexcept;
	void SetDirection(const DirectX::XMVECTOR & NewDirection) noexcept;
	void SetUpDirection(const DirectX::XMVECTOR & NewUpDirection) noexcept;

	DirectX::XMVECTOR Velocity;
	float AngleRotation = 0.0f;
	DirectX::XMVECTOR Direction;

	enum CAMERA_TYPE {
		FIRST,
		THIRD
	};

	CAMERA_TYPE type;

private:
	DirectX::XMVECTOR Position;


	DirectX::XMVECTOR UpDirection;
	
	int time = 0;


	bool waitForSwap = false;
	bool WaitForSwapJump = false;

	void swapCameraMode();

};


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

private:
	DirectX::XMVECTOR Position;
	DirectX::XMVECTOR Direction;
	DirectX::XMVECTOR UpDirection;
	float AngleRotation = 0.0f;

};


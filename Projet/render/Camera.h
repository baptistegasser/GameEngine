#pragma once

#include "core/Component.h"

class Camera : public Pitbull::Component {
protected:
	friend class Pitbull::Actor;
	Camera(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Position, const DirectX::XMVECTOR& Direction, const DirectX::XMVECTOR& UpDirection, DirectX::XMMATRIX* PMatView, DirectX::XMMATRIX* PMatProj, DirectX::XMMATRIX* PMatViewProj);

public:
	~Camera() = default;

	void Tick(const float& DeltaTime) override;

	void SetPosition(const DirectX::XMVECTOR& Position) noexcept;
	void SetDirection(const DirectX::XMVECTOR& Direction) noexcept;
	void SetUpDirection(const DirectX::XMVECTOR& UpDirection) noexcept;

private:
	bool NeedToUpdate;
	DirectX::XMVECTOR Position;
	DirectX::XMVECTOR Direction;
	DirectX::XMVECTOR UpDirection;
	DirectX::XMMATRIX* PMatView;
	DirectX::XMMATRIX* PMatProj;
	DirectX::XMMATRIX* PMatViewProj;

	// TODO
	float FOV, AspectRatio, NearClip, FarClip;
};

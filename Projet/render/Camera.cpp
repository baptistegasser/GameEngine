#include "stdafx.h"
#include "Camera.h"

using namespace DirectX;

Camera::Camera(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Position, const DirectX::XMVECTOR& Direction, const DirectX::XMVECTOR& UpDirection, DirectX::XMMATRIX* PMatView, DirectX::XMMATRIX* PMatProj, DirectX::XMMATRIX* PMatViewProj)
	: Component{ Parent }
	, Position{ Position }
	, Direction{ XMVector3Normalize(Direction) }
	, UpDirection{ XMVector3Normalize(UpDirection) }
	, PMatView{ PMatView }
	, PMatProj{ PMatProj }
	, PMatViewProj{ PMatViewProj }
{}

void Camera::Tick(const float& DeltaTime)
{
	// Matrice de la vision
	*PMatView = XMMatrixLookAtLH(Position, Position + Direction, UpDirection);

	// Recalculer matViewProj
	*PMatViewProj = (*PMatView) * (*PMatProj);
}

void Camera::SetPosition(const DirectX::XMVECTOR& NewPosition) noexcept
{
	Position = NewPosition;
}

void Camera::SetDirection(const DirectX::XMVECTOR& NewDirection) noexcept
{
	Direction = NewDirection;
}

void Camera::SetUpDirection(const DirectX::XMVECTOR& NewUpDirection) noexcept
{
	UpDirection = NewUpDirection;
}

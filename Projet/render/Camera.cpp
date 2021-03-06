#include "stdafx.h"
#include "Camera.h"
#include <gameplay/Player.h>
#include <core/Actor.h>
#include "math/Math.h"
//#include "EngineD3D11.h"


using namespace DirectX;

Camera::Camera(Pitbull::Actor* Parent, const XMVECTOR& Position, const XMVECTOR& Direction, const XMVECTOR& UpDirection, XMMATRIX* PMatView, XMMATRIX* PMatProj, XMMATRIX* PMatViewProj)
	: Component{ Parent }
	, NeedToUpdate{ true }
	, Position{ Position }
	, Direction{ XMVector3Normalize(Direction) }
	, UpDirection{ XMVector3Normalize(UpDirection) }
	, PMatView{ PMatView }
	, PMatProj{ PMatProj }
	, PMatViewProj{ PMatViewProj }
{}

void Camera::Tick(const float& ElapsedTime)
{
	// Calc view matrix
	*PMatView = XMMatrixLookAtLH(Position, Position + Direction, UpDirection);

	// Calc the view projection
	*PMatViewProj = (*PMatView) * (*PMatProj);

	// Done updating until next change
	NeedToUpdate = false;
}

void Camera::SetPosition(const DirectX::XMVECTOR& Position) noexcept
{
	this->Position = Position;
	NeedToUpdate = true;
}

void Camera::SetDirection(const DirectX::XMVECTOR& Direction) noexcept
{
	this->Direction = Direction;
	NeedToUpdate = true;
}

void Camera::SetUpDirection(const DirectX::XMVECTOR& UpDirection) noexcept
{
	this->UpDirection = UpDirection;
	NeedToUpdate = true;
}

const XMVECTOR& Camera::GetPosition() const noexcept
{
	return Position;
}

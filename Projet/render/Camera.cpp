#include "stdafx.h"
#include "Camera.h"
#include <render/Player.h>
#include <core/Actor.h>
#include "../math/Math.h"
//#include "MoteurWindows.h"


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
	auto player = ParentActor->GetComponent<Player>();
	if (player != nullptr) {
		if (player->type == Player::CAMERA_TYPE::THIRD) {
			SetPosition(Math::PX2XMVector(ParentActor->Transform.p) - player->Direction * 1.5 + XMVectorSet(0, 1.5, 0, 0));
		}
		else {
			SetPosition(Math::PX2XMVector(ParentActor->Transform.p) + player->Direction * 1.5 + XMVectorSet(0, 1.5, 0, 0));
		}
		SetDirection(player->Direction);	
	}

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

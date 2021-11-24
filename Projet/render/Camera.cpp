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
			Position = Math::PX2XMVector(ParentActor->Transform.p) - player->Direction * 1.5 + XMVectorSet(0, 0.75f, 0, 0);
		}
		else {
			Position = Math::PX2XMVector(ParentActor->Transform.p) + player->Direction + XMVectorSet(0, 0.75f, 0, 0);
		}
		Direction = player->Direction;	
	}

	// Matrice de la vision
	*PMatView = XMMatrixLookAtLH(Position, Position + Direction, UpDirection);

	// Recalculer matViewProj
	*PMatViewProj = (*PMatView) * (*PMatProj);
}

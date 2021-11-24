#include "stdafx.h"
#include "Player.h"
#include "MoteurWindows.h"
#include "../math/Math.h"

Player::Player(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Direction)
: Component{ Parent }
, Direction{ XMVector3Normalize(Direction) }
, type{ CAMERA_TYPE::THIRD }
, WaitForSwap{false}
{
}

void Player::Tick(const float& DeltaTime)
{
	PM3D::CDIManipulateur& rGestionnaireDeSaisie = PM3D::CMoteurWindows::GetInstance().GetGestionnaireDeSaisie();

	RelativeZ = XMVector3Normalize(XMVector3Cross(Direction, XMVECTOR{0, 1, 0}));

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_A)) {
		Left = true;
	}
	else {
		Left = false;
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_D)) {
		Right = true;
	}
	else {
		Right = false;
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_W)) {
		Forward = true;
	}
	else {
		Forward = false;
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_S)) {
		Backward = true;
	}
	else {
		Backward = false;
	}

 	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_SPACE)) {
		Jump = true;
	}
	else {
		Jump = false;
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_M)) {
		WaitForSwap = true;
	}
	else {
		if (WaitForSwap) SwapCameraMode();
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_LEFT)) {
		AngleRotation -= RotationSpeed;
		PxQuat qx = PxQuat(-RotationSpeed, PxVec3(0, 1, 0));
		Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
		Direction = XMVector4Normalize( Direction);
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_RIGHT)) {
		AngleRotation += RotationSpeed;
		PxQuat qx = PxQuat(RotationSpeed, PxVec3(0, 1, 0));
		Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
		Direction = XMVector4Normalize(Direction);
	}
}

void Player::SwapCameraMode()
{
	if (type == CAMERA_TYPE::FIRST) type = CAMERA_TYPE::THIRD;
	else type = CAMERA_TYPE::FIRST;

	WaitForSwap = false;
}


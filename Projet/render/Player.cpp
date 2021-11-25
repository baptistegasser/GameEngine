#include "stdafx.h"
#include "Player.h"
#include "MoteurWindows.h"
#include "../math/Math.h"
#include "physic/RigidBody.h"

Player::Player(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Direction)
: Component{ Parent }
, Direction{ XMVector3Normalize(Direction) }
, type{ CAMERA_TYPE::THIRD }
, WaitForSwap{false}
{
}

void Player::Init()
{
	// Get the component only once at init
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
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
	if (Forward) {
		MyRigidBody->AddForce(Math::XMVector2PX(Direction) * Speed, ForceMode::Impulse);
	}
	if (Backward) {
		MyRigidBody->AddForce(-Math::XMVector2PX(Direction) * Speed, ForceMode::Impulse);
	}
	if (Left) {
		MyRigidBody->AddForce(Math::XMVector2PX(RelativeZ) * Speed, ForceMode::Impulse);
	}
	if (Right) {
		MyRigidBody->AddForce(-Math::XMVector2PX(RelativeZ) * Speed, ForceMode::Impulse);
	}
	if (Jump) {
		MyRigidBody->AddForce(PxVec3(0.0f, 1.0f, 0.0f) * JumpSpeed, ForceMode::Impulse);
	}
}

void Player::SwapCameraMode()
{
	if (type == CAMERA_TYPE::FIRST) type = CAMERA_TYPE::THIRD;
	else type = CAMERA_TYPE::FIRST;

	WaitForSwap = false;
}


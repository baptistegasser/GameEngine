#include "stdafx.h"
#include "Player.h"

#include "render/MoteurWindows.h"
#include "math/Math.h"
#include "math/Vec3f.h"

using namespace Math;

Player::Player(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Direction)
	: Component{ Parent }
	, Direction{ XMVector3Normalize(Direction) }
	, ViewType{ CameraViewType::Third }
	, WaitForSwap{false}
{}

void Player::Init()
{
	// Get the needed components only once at init
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
	MyCamera = ParentActor->GetComponent<Camera>();
}

void Player::FixedTick(const float& DeltaTime)
{
	using namespace DirectX;

	PM3D::CDIManipulateur& rGestionnaireDeSaisie = PM3D::CMoteurWindows::GetInstance().GetGestionnaireDeSaisie();

	RelativeZ = XMVector3Normalize(XMVector3Cross(Direction, XMVECTOR{0, 1, 0}));

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_A)) {
		MyRigidBody->AddForce(Math::XMVector2PX(RelativeZ) * Speed * DeltaTime, ForceMode::Impulse);
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_D)) {
		MyRigidBody->AddForce(-Math::XMVector2PX(RelativeZ) * Speed * DeltaTime, ForceMode::Impulse);
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_W)) {
		MyRigidBody->AddForce(Math::XMVector2PX(Direction) * Speed * DeltaTime, ForceMode::Impulse);
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_S)) {
		MyRigidBody->AddForce(-Math::XMVector2PX(Direction) * Speed * DeltaTime, ForceMode::Impulse);
	}

 	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_SPACE)) {
		MyRigidBody->AddForce(Vec3f(0.0f, 1.0f, 0.0f) * JumpSpeed, ForceMode::Impulse);
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_M)) {
		WaitForSwap = true;
	}
	else {
		if (WaitForSwap) SwapCameraMode();
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_LEFT)) {
		using namespace physx;

		AngleRotation -= RotationSpeed;
		PxQuat qx = PxQuat(-RotationSpeed, PxVec3(0, 1, 0));
		Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
		Direction = XMVector4Normalize( Direction);
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_RIGHT)) {
		using namespace physx;

		AngleRotation += RotationSpeed;
		PxQuat qx = PxQuat(RotationSpeed, PxVec3(0, 1, 0));
		Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
		Direction = XMVector4Normalize(Direction);
	}

	if (ViewType == CameraViewType::Third) {
		MyCamera->SetPosition(Math::PX2XMVector(ParentActor->Transform.Position) - Direction * 1.5 + XMVectorSet(0, 0.75f, 0, 0));
	}
	else {
		MyCamera->SetPosition(Math::PX2XMVector(ParentActor->Transform.Position) + Direction + XMVectorSet(0, 0.75f, 0, 0));
	}
	MyCamera->SetDirection(Direction);
}

void Player::SwapCameraMode()
{
	if (ViewType == CameraViewType::First)
		ViewType = CameraViewType::Third;
	else
		ViewType = CameraViewType::First;

	WaitForSwap = false;
}


#include "stdafx.h"
#include "Player.h"

#include "render/MoteurWindows.h"
#include "math/Math.h"
#include "math/Vec3f.h"

using namespace Math;

Player::Player(Pitbull::Actor* Parent)
	: Component{ Parent }
	, ViewType{ CameraViewType::Third }
	, Direction{}
{}

void Player::Init()
{
	// Get the needed components only once at init
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
	MyCamera = ParentActor->GetComponent<Camera>();
	MyCollider = ParentActor->GetComponent<SphereCollider>();

	Direction = ParentActor->Transform.Forward().ToXMVector();
}

void Player::FixedTick(const float& DeltaTime)
{
	using namespace DirectX;

	auto& Engine = PM3D::CMoteurWindows::GetInstance();
	PM3D::CDIManipulateur& rGestionnaireDeSaisie = Engine.GetGestionnaireDeSaisie();

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
	    if (isGrounded())
			MyRigidBody->AddForce(Vec3f(0.0f, 1.0f, 0.0f) * JumpSpeed, ForceMode::Impulse);
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_P)) {
		Engine.IsPaused() ? Engine.UnPause() : Engine.Pause();
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_ESCAPE)) {
		Engine.Stop();
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

bool Player::isGrounded() const
{
	static auto& Scene = PM3D::CMoteurWindows::GetInstance().GetScene();

	auto Origin = ParentActor->Transform.Position;
	Origin.y -= MyCollider->Radius + 0.1f; // Little more than radius
	auto Hit = Scene.Raycast(
		Origin,
		{0.0f, -1.0f, 0.0f}, 
		0.1f);
	return Hit.hasAnyHits();
}

void Player::SwapCameraMode()
{
	if (ViewType == CameraViewType::First)
		ViewType = CameraViewType::Third;
	else
		ViewType = CameraViewType::First;

	WaitForSwap = false;
}

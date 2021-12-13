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

void Player::Tick(const float& ElapsedTime)
{
	if (IsDead())
	{
		ResetPlayer(Math::Vec3f(0, 10.5f, 0));
	}
}

void Player::FixedTick(const float& DeltaTime)
{
	using namespace DirectX;

	auto& Engine = PM3D::CMoteurWindows::GetInstance();
	PM3D::CDIManipulateur& rGestionnaireDeSaisie = Engine.GetGestionnaireDeSaisie();

	RelativeZ = XMVector3Normalize(XMVector3Cross(Direction, XMVECTOR{0, 1, 0}));

	/****
	 *  KEYBOARD CONTROL
	 ****/

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

	/****
	 *  MOUSE CONTROL
	 ****/

	// Calculate sensibility of the camera
	auto CalculateSpeed = [](int speed) { return 1000.f * static_cast<float>(speed) + 2000.f; };

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_LEFT)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(-XM_PI / (CalculateSpeed(SensibilityHorizontal) * DeltaTime)));
	}

	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_RIGHT)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(XM_PI / (CalculateSpeed(SensibilityHorizontal) * DeltaTime)));
	}

	//Vérifier si déplacement vers la gauche
	if (rGestionnaireDeSaisie.EtatSouris().lX < 0) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(-XM_PI / (CalculateSpeed(SensibilityHorizontal) * DeltaTime)));
	}

	// Vérifier si déplacement vers la droite
	if (rGestionnaireDeSaisie.EtatSouris().lX > 0) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(XM_PI / (CalculateSpeed(SensibilityHorizontal) * DeltaTime)));
	}

	//Vérifier si déplacement vers le haut
	if (rGestionnaireDeSaisie.EtatSouris().lY < 0 && AngleRotationVertical > MIN_ANGLE_VERTICAL) {
		AngleRotationVertical -= AngleRotationSpeedVertical;
		Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(RelativeZ, XM_PI / (CalculateSpeed(SensibilityVertical) * DeltaTime)));
	}

	// Vérifier si déplacement vers le bas
	if (rGestionnaireDeSaisie.EtatSouris().lY > 0 && AngleRotationVertical < MAX_ANGLE_VERTICAL) {
		AngleRotationVertical += AngleRotationSpeedVertical;
		Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(RelativeZ, -XM_PI / (CalculateSpeed(SensibilityVertical) * DeltaTime)));
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

bool Player::IsDead() const
{
	if (ParentActor->Transform.Position.y < -10) return true;
	return false;
}

void Player::ResetPlayer(Math::Vec3f Pos) const
{
	ParentActor->Transform = Transform(Pos,Math::Quaternion(0.0f,0.0f,0.0f));
	MyRigidBody->ClearForce();
	MyRigidBody->ClearTorque();
	MyRigidBody->ClearVelocity();
	MyRigidBody->ClearAngularVelocity();
}

#include "stdafx.h"
#include "Player.h"

#include "render/EngineD3D11.h"
#include "math/Math.h"
#include "math/Vec3f.h"

#include "physic/ContactFilter.h"

using namespace Math;

Player::Player(Pitbull::Actor* Parent, Vec3f Pos)
	: Component{ Parent }
	, ViewType{ CameraViewType::Third }
	, Direction{}
	, SpawnPos { Pos }
{
}

void Player::Init()
{
	// Get the needed components only once at init
	MyRigidBody = ParentActor->GetComponent<RigidBody>();
	MyCamera = ParentActor->GetComponent<Camera>();
	MyCollider = ParentActor->GetComponent<SphereCollider>();

	Direction = ParentActor->Transform.Forward().ToXMVector();

	MyRigidBody->setMaxLinearVelocity(MaxSpeed);
}

void Player::Tick(const float& ElapsedTime)
{
	if (IsDead())
	{
		RespawnPlayer();
	}
}

void Player::FixedTick(const float& DeltaTime)
{
	using namespace DirectX;

	auto& Engine = EngineD3D11::GetInstance();
	PM3D::CDIManipulateur& InputManager = Engine.InputManager;

	RelativeZ = XMVector3Normalize(XMVector3Cross(Direction, XMVECTOR{0, 1, 0}));

	if (InputManager.ToucheAppuyee(DIK_A)) {
		MyRigidBody->AddForce(Math::XMVector2PX(RelativeZ) * Speed * DeltaTime, ForceMode::Impulse);
	}

	if (InputManager.ToucheAppuyee(DIK_D)) {
		MyRigidBody->AddForce(-Math::XMVector2PX(RelativeZ) * Speed * DeltaTime, ForceMode::Impulse);
	}

	if (InputManager.ToucheAppuyee(DIK_W)) {
		MyRigidBody->AddForce(Math::XMVector2PX(Direction) * Speed * DeltaTime, ForceMode::Impulse);
	}

	if (InputManager.ToucheAppuyee(DIK_S)) {
		MyRigidBody->AddForce(-Math::XMVector2PX(Direction) * Speed * DeltaTime, ForceMode::Impulse);
	}

 	if (InputManager.ToucheAppuyee(DIK_SPACE)) {
	    if (isGrounded())
			MyRigidBody->AddForce(Vec3f(0.0f, 1.0f, 0.0f) * JumpSpeed, ForceMode::Impulse);
	}

	if (InputManager.ToucheAppuyee(DIK_P)) {
		Engine.IsPaused() ? Engine.UnPause() : Engine.Pause();
	}

	if (InputManager.ToucheAppuyee(DIK_ESCAPE)) {
		Engine.Stop();
	}

	if (InputManager.ToucheAppuyee(DIK_M)) {
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

	if (InputManager.ToucheAppuyee(DIK_LEFT)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(-XM_PI / (CalculateSpeed(SensibilityHorizontal) * DeltaTime)));
	}

	if (InputManager.ToucheAppuyee(DIK_RIGHT)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(XM_PI / (CalculateSpeed(SensibilityHorizontal) * DeltaTime)));
	}

	//V�rifier si d�placement vers la gauche
	if (InputManager.EtatSouris().lX < 0) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(-XM_PI / (CalculateSpeed(SensibilityHorizontal) * DeltaTime)));
	}

	// V�rifier si d�placement vers la droite
	if (InputManager.EtatSouris().lX > 0) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(XM_PI / (CalculateSpeed(SensibilityHorizontal) * DeltaTime)));
	}

	//V�rifier si d�placement vers le haut
	if (InputManager.EtatSouris().lY < 0 && AngleRotationVertical > MIN_ANGLE_VERTICAL) {
		AngleRotationVertical -= AngleRotationSpeedVertical;
		Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(RelativeZ, XM_PI / (CalculateSpeed(SensibilityVertical) * DeltaTime)));
	}

	// V�rifier si d�placement vers le bas
	if (InputManager.EtatSouris().lY > 0 && AngleRotationVertical < MAX_ANGLE_VERTICAL) {
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
	static auto& Scene = EngineD3D11::GetInstance().GetScene();

	auto Origin = ParentActor->Transform.Position;
	Origin.y -= MyCollider->Radius + 0.1f; // Little more than radius
	auto Hit = Scene.Raycast(
		Origin,
		{0.0f, -1.0f, 0.0f}, 
		0.1f);
	//return Hit.hasAnyHits();
	return true;
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

void Player::RespawnPlayer() const
{
	ParentActor->Transform = Transform(SpawnPos,Math::Quaternion(0.0f,0.0f,0.0f));
	MyRigidBody->ClearForce();
	MyRigidBody->ClearTorque();
	MyRigidBody->ClearVelocity();
	MyRigidBody->ClearAngularVelocity();
}

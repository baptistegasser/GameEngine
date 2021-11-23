#include "stdafx.h"
#include "Player.h"
#include "MoteurWindows.h"
#include "../math/Math.h"

Player::Player(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Direction, const DirectX::XMVECTOR& UpDirection)
: Component{ Parent }
, Position{ Math::PX2XMVector(Parent->Transform.p) }
, Direction{ XMVector3Normalize(Direction) }
, UpDirection{ XMVector3Normalize(UpDirection) }
, type{ CAMERA_TYPE::THIRD }
, waitForSwap{false}
, Velocity {0, 0, 0, 1}
{
}

void Player::Tick(const float& DeltaTime)
{
	// Pour les mouvements, nous utilisons le gestionnaire de saisie
	PM3D::CDIManipulateur& rGestionnaireDeSaisie = PM3D::CMoteurWindows::GetInstance().GetGestionnaireDeSaisie();

	float coeffMove = 0.5f;
	float RotationSpeed = 0.05f;
	XMVECTOR relativeZ = XMVector3Normalize(XMVector3Cross(Direction, XMVECTOR{0, 1, 0}));

	//// Vérifier l’état de la touche gauche
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_A)) {
	//	Position += (coeffMove * relativeZ * DeltaTime);
	//	Velocity += relativeZ * coeffMove * DeltaTime;
	//}

	//// Vérifier l’état de la touche droite
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_D)) {
	//	Position -= (coeffMove * relativeZ * DeltaTime);
	//	Velocity -= relativeZ * coeffMove * DeltaTime;
	//}

	// Vérifier l'état de la touche forward
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_W)) {
		Position += (coeffMove * Direction * DeltaTime);
		Velocity += Direction * coeffMove * DeltaTime;
		Velocity = XMVectorSetY(Velocity, 0);
	}

	// Vérifier l’état de la touche backward
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_S)) {
		Position -= (coeffMove * Direction * DeltaTime);
		Velocity -= Direction * coeffMove * DeltaTime;
		Velocity = XMVectorSetY(Velocity, 0);
	}


 	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_SPACE)) {
		Velocity = XMVectorSetY(Velocity, 10 *coeffMove * DeltaTime);
		time = 0;
	}
	else if (time > 100){
		Velocity = XMVectorSetY(Velocity, 0);
	}
	time++;

	// Vérifier l’état de la touche SwapMode
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_M)) {
		waitForSwap = true;
	}
	else {
		if (waitForSwap) swapCameraMode();
	}

	// ******** POUR LES FLECHES ************ 
	//Vérifier si déplacement vers la gauche
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_LEFT)) {
		AngleRotation -= RotationSpeed;
		PxQuat qx = PxQuat(-RotationSpeed, PxVec3(0, 1, 0));
		Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
		XMVector4Normalize( Direction);
	}

	// Vérifier si déplacement vers la droite
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_RIGHT)) {
		AngleRotation += RotationSpeed;
		PxQuat qx = PxQuat(RotationSpeed, PxVec3(0, 1, 0));
		Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
		XMVector4Normalize(Direction);
	}

	// fonctionne pas bien
	//// ******** POUR LA SOURIS ************  
	////Vérifier si déplacement vers la gauche
	//if (rGestionnaireDeSaisie.EtatSouris().lX < 0) {
	//	AngleRotation -= 0.5f;
	//	PxQuat qx = PxQuat(-0.5f, PxVec3(0, 1, 0));
	//	Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
	//	XMVector4Normalize(Direction);
	//}

	//// Vérifier si déplacement vers la droite
	//if (rGestionnaireDeSaisie.EtatSouris().lX > 0) {
	//	AngleRotation += 0.5f;
	//	PxQuat qx = PxQuat(0.5f, PxVec3(0, 1, 0));
	//	Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
	//	XMVector4Normalize(Direction);
	//}


	/*ParentActor->Transform = PxTransform(Math::XMVector2PX(Position), PxQuat(AngleRotation, PxVec3(0, 1, 0)));

	auto camera = ParentActor->GetComponent<Camera>();
	if (type == CAMERA_TYPE::THIRD) {
		camera->SetPosition(Position - Direction * 1.5 + XMVectorSet(0, 1.5, 0, 0));
	}
	else {
		camera->SetPosition(Position + Direction + XMVectorSet(0, 1.5, 0, 0));
	}
	camera->SetDirection(Direction);*/
}

void Player::SetPosition(const DirectX::XMVECTOR& NewPosition) noexcept
{
	Position = NewPosition;
}

void Player::SetDirection(const DirectX::XMVECTOR& NewDirection) noexcept
{
	Direction = NewDirection;
}

void Player::SetUpDirection(const DirectX::XMVECTOR& NewUpDirection) noexcept
{
	UpDirection = NewUpDirection;
}

void Player::swapCameraMode()
{
	if (type == CAMERA_TYPE::FIRST) type = CAMERA_TYPE::THIRD;
	else type = CAMERA_TYPE::FIRST;

	waitForSwap = false;
}


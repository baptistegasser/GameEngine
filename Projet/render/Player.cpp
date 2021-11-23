#include "stdafx.h"
#include "Player.h"
#include "MoteurWindows.h"
#include "../math/Math.h"

Player::Player(Pitbull::Actor* Parent, const DirectX::XMVECTOR& Direction, const DirectX::XMVECTOR& UpDirection)
: Component{ Parent }
, Position{ Math::PX2XMVector(Parent->Transform.p) }
, Direction{ XMVector3Normalize(Direction) }
, UpDirection{ XMVector3Normalize(UpDirection) }
{
}

void Player::Tick(const float& DeltaTime)
{
	// Pour les mouvements, nous utilisons le gestionnaire de saisie
	PM3D::CDIManipulateur& rGestionnaireDeSaisie = PM3D::CMoteurWindows::GetInstance().GetGestionnaireDeSaisie();

	float coeffMove = 50.0f;
	XMVECTOR relativeZ = XMVector3Normalize(XMVector3Cross(Direction, UpDirection));

	// Vérifier l’état de la touche gauche
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_A)) {
		Position += (coeffMove * relativeZ * DeltaTime);
	}

	// Vérifier l’état de la touche droite
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_D)) {
		Position -= (coeffMove * relativeZ * DeltaTime);
	}

	// Vérifier l'état de la touche forward
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_W)) {
		Position += (coeffMove * Direction * DeltaTime);
	}

	// Vérifier l’état de la touche backward
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_S)) {
		Position -= (coeffMove * Direction * DeltaTime);
	}

	//// Vérifier l’état de la touche SwapMode
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_M)) {
	//	waitForSwap = true;
	//}
	//else {
	//	if (waitForSwap) swapCameraMode();
	//}

	// ******** POUR LES FLECHES ************ 
	//Vérifier si déplacement vers la gauche
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_LEFT)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(-XM_PI / (5000.0f * DeltaTime)));
		AngleRotation -= 0.05f;
		PxQuat qx = PxQuat(-0.05f, PxVec3(0, 1, 0));
		Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
		XMVector4Normalize( Direction);
	}

	// Vérifier si déplacement vers la droite
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_RIGHT)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(XM_PI / (5000.0f * DeltaTime)));
		AngleRotation += 0.05f;
		PxQuat qx = PxQuat(0.05f, PxVec3(0, 1, 0));
		Direction = Math::PX2XMVector(qx.rotate(Math::XMVector2PX(Direction)));
		XMVector4Normalize(Direction);
	}

	//Vérifier si déplacement vers le haut
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_UP)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(relativeZ, XM_PI / (5000.0f * DeltaTime)));
	}

	// Vérifier si déplacement vers le bas
	if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_DOWN)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(relativeZ, -XM_PI / (5000.0f * DeltaTime)));
	}

	// ******** POUR LA SOURIS ************  
	//Vérifier si déplacement vers la gauche
	if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) && (rGestionnaireDeSaisie.EtatSouris().lX < 0)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(-XM_PI / (5000.0f * DeltaTime)));
	}

	// Vérifier si déplacement vers la droite
	if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) && (rGestionnaireDeSaisie.EtatSouris().lX > 0)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationY(XM_PI / (5000.0f * DeltaTime)));
	}

	//Vérifier si déplacement vers le haut
	if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) && (rGestionnaireDeSaisie.EtatSouris().lY < 0)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(relativeZ, XM_PI / (5000.0f * DeltaTime)));
	}

	// Vérifier si déplacement vers le bas
	if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) && (rGestionnaireDeSaisie.EtatSouris().lY > 0)) {
		Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(relativeZ, -XM_PI / (5000.0f * DeltaTime)));
	}

	//ParentActor->Transform.p = Math::XMVector2PX(Position);
	//ParentActor->Transform.q = Math::XMVector2Quat(Direction);

	ParentActor->Transform = PxTransform(Math::XMVector2PX(Position), PxQuat(AngleRotation, PxVec3(0, 1, 0)));

	/*auto camera = ParentActor->GetComponent<Camera>();
	camera->SetPosition(Position - Direction*1.5 + XMVectorSet(0, 1.5, 0, 0));
	camera->SetDirection(Direction);*/
	//camera->SetUpDirection(UpDirection);

	//camera.setEye(vehicule.mEye - vehicule.mDir * 40 + PxVec3(0, 15, 0));
	//camera.setDir(vehicule.mDir);
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

#include "stdafx.h"
#include "Camera.h"
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

	//// Pour les mouvements, nous utilisons le gestionnaire de saisie
	//PM3D::CDIManipulateur& rGestionnaireDeSaisie = PM3D::CMoteurWindows::GetInstance().GetGestionnaireDeSaisie();

	//float coeffMove = 500.0f;
	//XMVECTOR relativeZ = XMVector3Normalize(XMVector3Cross(Direction, UpDirection));


	//// V�rifier l��tat de la touche gauche
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_A)) {
	//	Position += (coeffMove * relativeZ * DeltaTime);
	//}

	//// V�rifier l��tat de la touche droite
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_D)) {
	//	Position -= (coeffMove * relativeZ * DeltaTime);
	//}

	//// V�rifier l'�tat de la touche forward
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_W)) {
	//	Position += (coeffMove * Direction * DeltaTime);
	//}

	//// V�rifier l��tat de la touche backward
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_S)) {
	//	Position -= (coeffMove * Direction * DeltaTime);
	//}

	////// V�rifier l��tat de la touche SwapMode
	////if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_M)) {
	////	waitForSwap = true;
	////}
	////else {
	////	if (waitForSwap) swapCameraMode();
	////}

	//// ******** POUR LES FLECHES ************ 
	////V�rifier si d�placement vers la gauche
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_LEFT)) {
	//	Direction = XMVector3Transform(Direction, XMMatrixRotationY(-XM_PI / (5000.0f * DeltaTime)));
	//}

	//// V�rifier si d�placement vers la droite
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_RIGHT)) {
	//	Direction = XMVector3Transform(Direction, XMMatrixRotationY(XM_PI / (5000.0f * DeltaTime)));
	//}

	////V�rifier si d�placement vers le haut
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_UP)) {
	//	Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(relativeZ, XM_PI / (5000.0f * DeltaTime)));
	//}

	//// V�rifier si d�placement vers le bas
	//if (rGestionnaireDeSaisie.ToucheAppuyee(DIK_DOWN)) {
	//	Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(relativeZ, -XM_PI / (5000.0f * DeltaTime)));
	//}

	//// ******** POUR LA SOURIS ************  
	////V�rifier si d�placement vers la gauche
	//if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) && (rGestionnaireDeSaisie.EtatSouris().lX < 0)) {
	//	Direction = XMVector3Transform(Direction, XMMatrixRotationY(-XM_PI / (5000.0f * DeltaTime)));
	//}

	//// V�rifier si d�placement vers la droite
	//if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) && (rGestionnaireDeSaisie.EtatSouris().lX > 0)) {
	//	Direction = XMVector3Transform(Direction, XMMatrixRotationY(XM_PI / (5000.0f * DeltaTime)));
	//}

	////V�rifier si d�placement vers le haut
	//if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) && (rGestionnaireDeSaisie.EtatSouris().lY < 0)) {
	//	Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(relativeZ, XM_PI / (5000.0f * DeltaTime)));
	//}

	//// V�rifier si d�placement vers le bas
	//if ((rGestionnaireDeSaisie.EtatSouris().rgbButtons[0] & 0x80) && (rGestionnaireDeSaisie.EtatSouris().lY > 0)) {
	//	Direction = XMVector3Transform(Direction, XMMatrixRotationAxis(relativeZ, -XM_PI / (5000.0f * DeltaTime)));
	//}

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

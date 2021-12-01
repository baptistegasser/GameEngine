#pragma once
#include "dispositif.h"

#include "AfficheurTexte.h"
#include "DIManipulateur.h"

#include "core/Actor.h"
#include "core/Scene.h"
#include "physic/PhysicManager.h"

#include "util/ResourcesManager.h"
#include "util/Singleton.h"

// Physic components
#include "physic/RigidBody.h"
#include "physic/SphereCollider.h"
#include "physic/BoxCollider.h"
#include "physic/CapsuleCollider.h"
#include "gameplay/Plateform.h"
// Render components
#include "render/MeshRenderer.h"
#include "render/Camera.h"
#include "gameplay/Player.h"
#include "render/Terrain.h"
// Gameplay components

// Sprite
#include "render/AfficheurSprite.h"

using namespace physx;

#include <iostream>
#include <chrono>

namespace PM3D
{

const int IMAGESPARSECONDE = 60;
const double EcartTemps = 1.0 / static_cast<double>(IMAGESPARSECONDE);

//
//   TEMPLATE : CMoteur
//
//   BUT : Template servant à construire un objet Moteur qui implantera les
//         aspects "génériques" du moteur de jeu
//
//   COMMENTAIRES :
//
//        Comme plusieurs de nos objets représenteront des éléments uniques 
//		  du système (ex: le moteur lui-même, le lien vers 
//        le dispositif Direct3D), l'utilisation d'un singleton 
//        nous simplifiera plusieurs aspects.
//
template <class T, class TClasseDispositif> class CMoteur :public Singleton<T>
{
public:

	virtual void Run()
	{
		bool bBoucle = true;

		while (bBoucle)
		{
			// Propre à la plateforme - (Conditions d'arrêt, interface, messages)
			bBoucle = RunSpecific();

			// appeler la fonction d'animation
			if (bBoucle)
			{
				bBoucle = Animation();
			}
		}
	}

	virtual int Initialisations()
	{
		// Propre à la plateforme
		InitialisationsSpecific();

		// * Initialisation du dispositif de rendu
		pDispositif = CreationDispositifSpecific(CDS_FENETRE);

		CurrentScene = std::make_shared<Scene>();

		PhysicManager::GetInstance().Init();
		PhysicManager::GetInstance().InitScene(CurrentScene);

		// * Initialisation de la scène
		InitScene();

		// * Initialisation des paramètres de l'animation et 
		//   préparation de la première image
		InitAnimation();

		return 0;
	}

	virtual bool Animation()
	{
		// méthode pour lire l'heure et calculer le 
		// temps écoulé
		const int64_t TempsCompteurCourant = GetTimeSpecific();
		const double TempsEcoule = GetTimeIntervalsInSec(TempsCompteurPrecedent, TempsCompteurCourant);

		const double PhysicElapsedTime = GetTimeIntervalsInSec(CurrentPhysicTime, TempsCompteurCourant);
		CurrentPhysicTime = TempsCompteurCourant;

		// Update physic state
		PhysicAccumulator += static_cast<float>(PhysicElapsedTime);
		while (PhysicAccumulator >= PhysicDeltaStep) {
			PhysicManager::GetInstance().Step(PhysicDeltaStep);
			CurrentScene->FixedTick(PhysicDeltaStep);
			PhysicAccumulator -= PhysicDeltaStep;
		}

		// Update inputs states
		GestionnaireDeSaisie.StatutClavier();
		GestionnaireDeSaisie.SaisirEtatSouris();

		// Est-il temps de rendre l'image?
		if (TempsEcoule > EcartTemps)
		{
			// Affichage optimisé
			pDispositif->Present();

			// On prépare la prochaine image
			AnimeScene(static_cast<float>(TempsEcoule));

			// On rend l'image sur la surface de travail
			// (tampon d'arrière plan)
			RenderScene(static_cast<float>(TempsEcoule));

			// Calcul du temps du prochain affichage
			TempsCompteurPrecedent = TempsCompteurCourant;
		}

		return true;
	}

	const DirectX::XMMATRIX& GetMatView() const { return m_MatView; }
	const DirectX::XMMATRIX& GetMatProj() const { return m_MatProj; }
	const DirectX::XMMATRIX& GetMatViewProj() const { return m_MatViewProj; }

	CDIManipulateur& GetGestionnaireDeSaisie() { return GestionnaireDeSaisie; }
	ResourcesManager& GetResourcesManager() { return ResourcesManager; }
	const Scene& GetScene() const noexcept { return *CurrentScene; }
	CDispositifD3D11& GetDispositif() noexcept { return *pDispositif; }

protected:
	~CMoteur()
	{
		Cleanup();
	}

	// Spécifiques - Doivent être implantés
	virtual bool RunSpecific() = 0;
	virtual int InitialisationsSpecific() = 0;

	virtual int64_t GetTimeSpecific() const = 0;
	virtual double GetTimeIntervalsInSec(int64_t start, int64_t stop) const = 0;

	virtual TClasseDispositif* CreationDispositifSpecific(const CDS_MODE cdsMode) = 0;
	virtual void BeginRenderSceneSpecific() = 0;
	virtual void EndRenderSceneSpecific() = 0;

	// Autres fonctions
	virtual int InitAnimation()
	{
		TempsSuivant = GetTimeSpecific();
		TempsCompteurPrecedent = TempsSuivant;
		CurrentPhysicTime = TempsSuivant;

		// première Image
		RenderScene(0.f);

		return true;
	}


	// Fonctions de rendu et de présentation de la scène
	virtual bool RenderScene(const float ElapsedTime)
	{
		BeginRenderSceneSpecific();

		// Get actors in vision range
		const auto Actors = CurrentScene->GetVisibleActors();
		for (const auto& Actor : Actors) {
			Actor->LateTick(ElapsedTime);
		}

		for (const auto& Actor : Actors) {
			Actor->SpriteTick(ElapsedTime);
		}

		EndRenderSceneSpecific();
		return true;
	}

	virtual void Cleanup()
	{
		ResourcesManager.Cleanup();
		PhysicManager::GetInstance().Cleanup();

		// Détruire le dispositif
		if (pDispositif)
		{
			delete pDispositif;
			pDispositif = nullptr;
		}
	}

	virtual int InitScene()
	{
		using namespace DirectX;

		// Create actors
		if (!InitObjets()) {
			return 1;
		}

		//// Initialisation des matrices View et Proj
		//// Dans notre cas, ces matrices sont fixes
		m_MatView = XMMatrixLookAtLH(
			XMVectorSet(0.0f, 3.0f, -5.0f, 1.0f),
			XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

		const float champDeVision = XM_PI / 2; 	// 45 degr�s
		const float ratioDAspect = static_cast<float>(pDispositif->GetLargeur()) / static_cast<float>(pDispositif->GetHauteur());
		const float planRapproche = 1.0;
		const float planEloigne = 300.0;

		m_MatProj = XMMatrixPerspectiveFovLH(
			champDeVision,
			ratioDAspect,
			planRapproche,
			planEloigne);

		// Calcul de VP a l'avance
		m_MatViewProj = m_MatView * m_MatProj;

		// Finally init the scene
		CurrentScene->Init();

		return 0;
	}

 	bool InitObjets()
	{
		using namespace DirectX;

		auto Terrain = std::unique_ptr<ATerrain>(new ATerrain{
			L".\\modeles\\Heightmap.bmp",
			{1, 0.3f, 1},
			ResourcesManager.GetShader(L".\\shaders\\MiniPhongTerrain.fx"),
			PhysicMaterial{ 0.5f, 0.5f, 0.2f}
		});
		Terrain->Texture1 = ResourcesManager.GetTexture(L".\\modeles\\gazon.dds");
		Terrain->Texture2 = ResourcesManager.GetTexture(L".\\modeles\\roche.dds");
		Terrain->Texture3 = ResourcesManager.GetTexture(L".\\modeles\\chemin.dds");
		Terrain->Transform.PosRot.p = { 0.f, -50.f, 0.f };
		CurrentScene->AddActor(std::move(Terrain), true);

		auto Mesh2 = Pitbull::Actor::New();
		Mesh2->AddComponent<MeshRenderer>(ResourcesManager.GetMesh(L".\\modeles\\cube\\cube.OMB" ), ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
		Mesh2->AddComponent<SphereCollider>(PhysicMaterial{ 0.5f, 0.5f, 1.0f }, 1.0f);
		Mesh2->Transform.PosRot.p.y = 0.f;
		Mesh2->Transform.PosRot.p.z = -7.f;
		Mesh2->Transform.PosRot.p.x = 1.f;
		Mesh2->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
		Mesh2->AddComponent<Plateform>(
			PxTransform(Mesh2->Transform.PosRot.p, PxQuat(-PxHalfPi, PxVec3(0,1,0)))
			, PxTransform(Mesh2->Transform.PosRot.p + PxVec3(10, 0, 0), PxQuat(PxHalfPi, PxVec3(0, 1, 0))), true);
		CurrentScene->AddActor(std::move(Mesh2));


		//CreatePlatform(PxVec3(5, 5, 5), PxVec3(6, 0.25, 6));
		CreatePlatform(PxVec3(0, -2.f, 0), PxVec3(20.f, 1.f, 20.f), PxVec3( 0.5f, 0.5f, 0.0f ));
		CreateMobilePlatform(PxVec3(10.f, 10.f, 0), PxVec3(1.f, 1.f, 1.f), PxVec3(10, 0, 0), L".\\modeles\\plateform\\plateformRouge.OMB", PxVec3(0.5f, 0.5f, 0.0f));
		CreateMobilePlatform(PxVec3(10.f, 10.f, 20.f), PxVec3(1.f, 1.f, 1.f), PxVec3(-10, 0, 0), L".\\modeles\\plateform\\plateformGlace.OMB", PxVec3(0.75f, 0.75f, 0.0f));
		//MyPlateform->AddComponent<Plateform>(PxTransform(PxVec3(5, 5, 5)), PxTransform(PxVec3(-3, 5, 5)), true);

		auto MyPlayer = Pitbull::Actor::New();
		MyPlayer->AddComponent<MeshRenderer>(ResourcesManager.GetMesh(L".\\modeles\\ball3\\ball.OMB"), ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
		MyPlayer->AddComponent<Player>(XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f));
		auto PlayerCam = MyPlayer->AddComponent<Camera>(XMVectorSet(0.0f, 2.0f, 10.0f, 1.0f),
			XMVectorSet(0.0f, 0.4f, -1.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
			&m_MatView,
			&m_MatProj,
			&m_MatViewProj);
		CurrentScene->SetCurrentCamera(PlayerCam);

		MyPlayer->AddComponent<SphereCollider>(PhysicMaterial{ 0.5f, 0.5f, 0.2f }, 1.0f);
		auto PlayerBody = MyPlayer->AddComponent<RigidBody>(RigidBody::RigidActorType::Dynamic);
		CurrentScene->AddActor(std::move(MyPlayer));

		PlayerBody->SetMass(10.f);

		CurrentScene->LightConfig.SetAmbient(AmbientLight{ 0.5f, 0.5f, 0.5f });
		PointLight Light;
		Light.Intensity = 4.0f;
		Light.Position = { 0.f, 20.f, 0.f };
		Light.Specular = { 0.5f, 0.5f, 0.5f };
		Light.Roughness = { 0.5f, 0.5f, 0.5f };
		CurrentScene->LightConfig.AddPointLight(Light);


		std::unique_ptr<CAfficheurSprite> pAfficheurSprite =
			std::make_unique<CAfficheurSprite>(pDispositif);
		

		pAfficheurSprite->AjouterSprite(L".\\modeles\\tree02s.dds", 200, 400);
		pAfficheurSprite->AjouterSprite(L".\\modeles\\tree02s.dds", 500, 500, 100, 100);
		//pAfficheurSprite->AjouterSprite(L".\\modeles\\tree02s.dds", 800, 200, 100, 100);
		pAfficheurSprite->AjouterPanneau(L".\\modeles\\tree02s.dds", XMFLOAT3{ 800, 200, 0 }, 100, 100);
		//pAfficheurSprite->AjouterPanneau(L".\\modeles\\tree02s.dds", XMFLOAT3{ 200, 0, 0 }, 1000, 1000);
		//pAfficheurSprite->AjouterPanneau(L".\\modeles\\grass_v1_basic_tex.dds", XMFLOAT3(1.0f, 0.0f, 1.0f));
		//pAfficheurSprite->AjouterSprite(L".\\modeles\\tree02s.dds", 800, 200, 100, 100);
		CurrentScene->AddActor(std::move(pAfficheurSprite));

		return true;
	}

	void CreatePlatform(PxVec3 Pos, PxVec3 Scale, PxVec3 Material)
	{
		auto MyPlateform = Pitbull::Actor::New();
		MyPlateform->AddComponent<MeshRenderer>(ResourcesManager.GetMesh(L".\\modeles\\platform\\plateform.OMB"), ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
		MyPlateform->AddComponent<BoxCollider>(
			PhysicMaterial{ 0.5f, 0.5f, 0.0f },
			PxVec3{ 7 * Scale.x, 0.20f * Scale.y, 7  * Scale.z }
		);
		MyPlateform->Transform.PosRot.p = Pos;
		MyPlateform->Transform.Scale = Scale;
		MyPlateform->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
		CurrentScene->AddActor(std::move(MyPlateform));
	}

	void CreateMobilePlatform(PxVec3 Pos, PxVec3 Scale, PxVec3 End, const wchar_t* Filename, PxVec3 Material)
	{
		auto MyPlateform = Pitbull::Actor::New();
		MyPlateform->AddComponent<MeshRenderer>(ResourcesManager.GetMesh(Filename), ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
		MyPlateform->AddComponent<BoxCollider>(
			PhysicMaterial{ Material.x, Material.y, Material.z },
			PxVec3{ 7 * Scale.x, 0.20f * Scale.y, 7 * Scale.z }
		);
		MyPlateform->Transform.PosRot.p = Pos;
		MyPlateform->Transform.Scale = Scale;
		MyPlateform->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
		MyPlateform->AddComponent<Plateform>(
			PxTransform(MyPlateform->Transform.PosRot.p)
			, PxTransform(MyPlateform->Transform.PosRot.p + End), true);
		CurrentScene->AddActor(std::move(MyPlateform));
	}

	bool AnimeScene(float tempsEcoule)
	{
		// Make all component Tick
		CurrentScene->Tick(tempsEcoule);

		// Update the scene to reflect components changes
		CurrentScene->Update();

		return true;
	}

protected:
	const float PhysicDeltaStep = 1.0f / 60.0f;
	float PhysicAccumulator = 0;
	int64_t CurrentPhysicTime;

	// Variables pour le temps de l'animation
	int64_t TempsSuivant;
	int64_t TempsCompteurPrecedent;

	// Le dispositif de rendu
	TClasseDispositif* pDispositif;

	// La seule scène
	std::shared_ptr<Scene> CurrentScene;

	// Les matrices
	DirectX::XMMATRIX m_MatView;
	DirectX::XMMATRIX m_MatProj;
	DirectX::XMMATRIX m_MatViewProj;

	// Pour le texte
	std::unique_ptr<CAfficheurTexte> pTexte1;
	std::wstring str;

	std::unique_ptr<Gdiplus::Font> pPolice;

	// Les saisies
	CDIManipulateur GestionnaireDeSaisie;
	ResourcesManager ResourcesManager;
};

} // namespace PM3D


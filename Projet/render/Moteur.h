#pragma once
#include "dispositif.h"

#include "Objet3D.h"
#include "Bloc.h"
#include "BlocEffet1.h"
#include "ObjetMesh.h"
#include "ChargeurOBJ.h"

#include "AfficheurSprite.h"
#include "AfficheurTexte.h"
#include "DIManipulateur.h"
#include "Terrain.h"

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
// Render components
#include "render/MeshRenderer.h"
#include "render/Camera.h"
// Gameplay components

using namespace physx;

#include <iostream>

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

		// Update physic state
		PhysicAccumulator += static_cast<float>(TempsEcoule);
		while (PhysicAccumulator >= PhysicDeltaStep) {
			PhysicManager::GetInstance().Step(PhysicDeltaStep);
			PhysicAccumulator -= PhysicDeltaStep;
		}

		// Update inputs states
		GestionnaireDeSaisie.StatutClavier();
		GestionnaireDeSaisie.SaisirEtatSouris();

		// Est-il temps de rendre l'image?
		if (TempsEcoule > EcartTemps)
		{
			// Affichage optimisé
			pDispositif->Present(); // On enlevera «//» plus tard

			// On prépare la prochaine image
			AnimeScene(static_cast<float>(TempsEcoule));

			// Update tree after movement
			CurrentScene->Update(static_cast<float>(TempsEcoule));

			// On rend l'image sur la surface de travail
			// (tampon d'arrière plan)
			RenderScene();

			// Calcul du temps du prochain affichage
			TempsCompteurPrecedent = TempsCompteurCourant;
		}

		return true;
	}

	const XMMATRIX& GetMatView() const { return m_MatView; }
	const XMMATRIX& GetMatProj() const { return m_MatProj; }
	const XMMATRIX& GetMatViewProj() const { return m_MatViewProj; }

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

		// première Image
		RenderScene();

		return true;
	}


	// Fonctions de rendu et de présentation de la scène
	virtual bool RenderScene()
	{
		BeginRenderSceneSpecific();

		for (auto& actor : CurrentScene->Tree.Find({0.f}, 10.f)) {
			// TODO Parent class for renderable
			const auto Components = actor->GetComponents<MeshRenderer>();
			for (auto& Comp : Components) {
				Comp->Tick(0.f);
			}
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
		//// Initialisation des objets 3D - création et/ou chargement
		//if (!InitObjets())
		//{
		//	return 1;
		//}

		///*
		//// Initialisation des matrices View et Proj
		//// Dans notre cas, ces matrices sont fixes
		//m_MatView = XMMatrixLookAtLH(XMVectorSet(-2000.0f, 2000.0f, 2000.0f, 1.0f),
		////m_MatView = XMMatrixLookAtLH(XMVectorSet(5.0f, 2.0f, 5.0f, 1.0f),
		//	XMVectorSet(2.0f, 2.0f, 2.0f, 1.0f),
		//	XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

		//// Calcul de VP à l'avance
		//m_MatViewProj = m_MatView * m_MatProj;
		//*/

		//const float champDeVision = XM_PI / 4; 	// 45 degrés
		//const float ratioDAspect = static_cast<float>(pDispositif->GetLargeur()) / static_cast<float>(pDispositif->GetHauteur());
		//const float planRapproche = 2.0f;
		//const float planEloigne = 10000.0f;

		//m_MatProj = XMMatrixPerspectiveFovLH(
		//	champDeVision,
		//	ratioDAspect,
		//	planRapproche,
		//	planEloigne);

		//camera.init(XMVectorSet(-2000.0f, 2000.0f, 2000.0f, 1.0f), XMVectorSet(1.0f, -1.0f, -1.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), &m_MatView, &m_MatProj, &m_MatViewProj, CCamera::CAMERA_TYPE::LEVEL);
		//camera.update();

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

		// Finnaly init the scene
		CurrentScene->Init();

		return 0;
	}

	// TODO Create actor
 	bool InitObjets()
	{
		auto Mesh = Pitbull::Actor::New();
		Mesh->AddComponent<MeshRenderer>(ResourcesManager.GetMesh(L".\\modeles\\jin\\jin.OMB"), ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
		Mesh->AddComponent<SphereCollider>(PhysicMaterial{ 0.5f, 0.5f, 1.0f }, 1.0f);
		Mesh->Transform.p.y = -1.f;
		Mesh->AddComponent<RigidBody>(true, true, 10.f);
		CurrentScene->AddActor(std::move(Mesh));

		auto Other = Pitbull::Actor::New();
		Other->Transform.p.y = 10.f;
		Other->Transform.p.z = 0.5f;
		Other->AddComponent<MeshRenderer>(ResourcesManager.GetMesh(L".\\modeles\\jin\\jin.OMB"), ResourcesManager.GetShader(L".\\shaders\\MiniPhong.fx"));
		Other->AddComponent<BoxCollider>(PhysicMaterial{ 0.5f, 0.5f, 1.5f }, physx::PxVec3{1.0f});
		Other->AddComponent<RigidBody>(false, false, 10.f);
		CurrentScene->AddActor(std::move(Other));

		auto MyCamera = Pitbull::Actor::New();
		MyCamera->AddComponent<Camera>(XMVectorSet(0.0f, -5.0f, 10.0f, 1.0f),
			XMVectorSet(0.0f, 0.4f, -1.0f, 1.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
			&m_MatView,
			&m_MatProj,
			&m_MatViewProj);
		CurrentScene->AddActor(std::move(MyCamera));

		/*camera = CCamera{XMVectorSet(0.0f, -10.0f, 10.0f, 1.0f),
			XMVectorSet(0.0f, 1.0f, -1.0f, 1.0f),
			XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
			&m_MatView,
			&m_MatProj,
			&m_MatViewProj };*/

		// Puis, il est ajouté à la scène
		//char* filename = new char[50]("./src/Heightmap.bmp");
		//ListeScene.emplace_back(std::make_unique<Terrain>(filename, XMFLOAT3(20.0f, 3.0f, 20.0f), pDispositif));

		
		////// Constructeur avec format binaire
		//std::unique_ptr<CObjetMesh> pMesh = std::make_unique<CObjetMesh>(".\\modeles\\jin\\jin.OMB", pDispositif);
		//// Puis, il est ajouté à la scène
		//ListeScene.push_back(std::move(pMesh));

		////// Création de l'afficheur de sprites et ajout des sprites
		//std::unique_ptr<CAfficheurSprite> pAfficheurSprite = std::make_unique<CAfficheurSprite>(pDispositif);

		//// ajout de panneaux 
		//pAfficheurSprite->AjouterPanneau("modeles\\grass_v1_basic_tex.dds",
		//	XMFLOAT3(1.0f, 0.0f, 1.0f));
		//pAfficheurSprite->AjouterPanneau("modeles\\grass_v1_basic_tex.dds",
		//	XMFLOAT3(0.0f, 0.0f, -1.0f));
		//pAfficheurSprite->AjouterPanneau("modeles\\grass_v1_basic_tex.dds",
		//	XMFLOAT3(-1.0f, 0.0f, 0.5f));
		//pAfficheurSprite->AjouterPanneau("modeles\\grass_v1_basic_tex.dds",
		//	XMFLOAT3(-0.5f, 0.0f, 1.0f));
		//pAfficheurSprite->AjouterPanneau("modeles\\grass_v1_basic_tex.dds",
		//	XMFLOAT3(-2.0f, 0.0f, 2.0f));

		//pAfficheurSprite->AjouterSprite("modeles\\tree02s.dds", 200,400);
		//pAfficheurSprite->AjouterSprite("modeles\\tree02s.dds", 500,500, 100, 100);
		//pAfficheurSprite->AjouterSprite("modeles\\tree02s.dds", 800,200, 100, 100);

		//CAfficheurTexte::Init();
		//const Gdiplus::FontFamily oFamily(L"Arial", nullptr);
		//pPolice = std::make_unique<Gdiplus::Font>(&oFamily, 16.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		//pTexte1 = std::make_unique<CAfficheurTexte>(pDispositif, 256, 256, pPolice.get());

		//pAfficheurSprite->AjouterSpriteTexte(pTexte1->GetTextureView(), 0, 257);

		//pTexte1->Ecrire(L"Test du texte");

		//// Puis, il est ajouté à la scène
		//ListeScene.push_back(std::move(pAfficheurSprite));

		return true;
	}

	bool AnimeScene(float tempsEcoule)
	{
		/*if (camera.getType() == CCamera::CAMERA_TYPE::LEVEL && camera.getPosition().vector4_f32[0] > (-terrain->width / 2.0f) * terrain->scale.x && camera.getPosition().vector4_f32[0] < (terrain->width / 2.0f) * terrain->scale.x
			&& camera.getPosition().vector4_f32[2] > (-terrain->height / 2.0f) * terrain->scale.z && camera.getPosition().vector4_f32[2] < (terrain->height / 2.0f) * terrain->scale.z) {

			float y = terrain->getHeight(camera.getPosition().vector4_f32[0], camera.getPosition().vector4_f32[2]) + CCamera::HEIGHT;

			for (auto& object3D : ListeScene)
			{
				object3D->Anime(tempsEcoule);
			}
			camera.update(y, tempsEcoule);
		}
		else {
			for (auto& object3D : ListeScene)
			{
				object3D->Anime(tempsEcoule);
			}
			camera.update(tempsEcoule);

		}*/

		//camera.update(tempsEcoule);

		return true;
	}

protected:
	const float PhysicDeltaStep = 1.0f / 60.0f;
	float PhysicAccumulator = 0;

	// Variables pour le temps de l'animation
	int64_t TempsSuivant;
	int64_t TempsCompteurPrecedent;

	// Le dispositif de rendu
	TClasseDispositif* pDispositif;

	// La seule scène
	std::shared_ptr<Scene> CurrentScene;

	// Les matrices
	XMMATRIX m_MatView;
	XMMATRIX m_MatProj;
	XMMATRIX m_MatViewProj;

	// Pour le texte
	std::unique_ptr<CAfficheurTexte> pTexte1;
	std::wstring str;

	std::unique_ptr<Gdiplus::Font> pPolice;

	// Les saisies
	CDIManipulateur GestionnaireDeSaisie;
	ResourcesManager ResourcesManager;

	// Le Terrain
	Terrain* terrain;
};

} // namespace PM3D


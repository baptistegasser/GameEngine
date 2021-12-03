#pragma once
#include "dispositif.h"

#include "AfficheurTexte.h"
#include "DIManipulateur.h"

#include "core/Actor.h"
#include "core/Scene.h"
#include "physic/PhysicManager.h"

#include "util/ResourcesManager.h"
#include "util/Singleton.h"

// Math components
#include "math/Math.h"
// Physic components
#include "physic/RigidBody.h"
#include "physic/SphereCollider.h"
#include "physic/BoxCollider.h"
#include "physic/CapsuleCollider.h"
// Render components
#include "render/MeshRenderer.h"
#include "render/Camera.h"
#include "render/Terrain.h"
// Gameplay components
#include "gameplay/Plateform.h"
#include "gameplay/Player.h"
#include "render/GameFactory.h"

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
			CurrentScene->FixedTick(PhysicDeltaStep);
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

	DirectX::XMMATRIX& GetMatView() { return m_MatView; }
	const DirectX::XMMATRIX& GetMatView() const { return m_MatView; }
	DirectX::XMMATRIX& GetMatProj() { return m_MatProj; }
	const DirectX::XMMATRIX& GetMatProj() const { return m_MatProj; }
	DirectX::XMMATRIX& GetMatViewProj() { return m_MatViewProj; }
	const DirectX::XMMATRIX& GetMatViewProj() const { return m_MatViewProj; }

	CDIManipulateur& GetGestionnaireDeSaisie() { return GestionnaireDeSaisie; }
	ResourcesManager& GetResourcesManager() { return ResourcesManager; }
	Scene& GetScene() { return *CurrentScene; }
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
		GameFactory::GetInstance().LoadLevel();

		return true;
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


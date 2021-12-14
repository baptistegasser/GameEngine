#pragma once
#include "Device.h"

#include "DeviceD3D11.h"
#include "DIManipulateur.h"

#include "core/Actor.h"
#include "core/Scene.h"
#include "physic/PhysicManager.h"

#include "util/ResourcesManager.h"
#include "util/Singleton.h"

#include "render/GameFactory.h"

// Sprite
#include "render/SpriteRenderer.h"
#include "render/TextRenderer.h"
#include "render/Timer.h"
#include "render/Speed.h"

#include <iostream>
#include <chrono>

namespace PM3D
{

const int IMAGESPARSECONDE = 60;
const double EcartTemps = 1.0 / static_cast<double>(IMAGESPARSECONDE);

/// <summary>
/// Represent the state of the engine.
/// As state are represent by bit flag, multiple state can be possible at any given time.
/// </summary>
enum EngineState
{
	Starting  = 1 << 0,
	Running	  = 1 << 1,
	Paused	  = 1 << 2,
	Stopping  = 1 << 3,
};

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
template <class T, class TDevice> class CMoteur :public Singleton<T>
{
public:

	virtual void Run()
	{
		// We have passed the init phase no we are running !
		UnsetState(EngineState::Starting);
		SetState(EngineState::Running);

		while (!IsStopping()) {
			// Platfom specific stuff, ignore pause state or the app might freeze
			// -> keep treating windows events, keyboard events...
			RunSpecific();

			// appeler la fonction d'animation
			Animation();
		}
	}

	virtual int Initialisations()
	{
		// Set the engine state.
		SetState(EngineState::Starting);

		// Propre à la plateforme
		InitialisationsSpecific();

		// * Initialisation du dispositif de rendu
		pDispositif = CreationDispositifSpecific(CDS_MODE::CDS_FENETRE);

		CurrentScene = new Scene;

		PhysicManager::GetInstance().Init();
		PhysicManager::GetInstance().InitScene(CurrentScene->PhysxScene);

		// * Initialisation de la scène
		InitScene();

		// * Initialisation des paramètres de l'animation et 
		//   préparation de la première image
		InitAnimation();
		
		return 0;
	}

	virtual void Animation()
	{
		// méthode pour lire l'heure et calculer le 
		// temps écoulé
		const int64_t TempsCompteurCourant = GetTimeSpecific();
		const double TempsEcoule = GetTimeIntervalsInSec(TempsCompteurPrecedent, TempsCompteurCourant);

		const double PhysicElapsedTime = GetTimeIntervalsInSec(CurrentPhysicTime, TempsCompteurCourant);
		CurrentPhysicTime = TempsCompteurCourant;

		// Update inputs states
		

		// Update physic state
		PhysicAccumulator += static_cast<float>(PhysicElapsedTime);
		while (PhysicAccumulator >= PhysicDeltaStep) {
			CurrentScene->FixedTick(PhysicDeltaStep);
			if (!IsPaused()) PhysicManager::GetInstance().Step(PhysicDeltaStep);
			PhysicAccumulator -= PhysicDeltaStep;
		}

		// Est-il temps de rendre l'image?
		if (TempsEcoule > EcartTemps)
		{
			// On prépare la prochaine image
			AnimeScene(static_cast<float>(TempsEcoule));

			if (!IsPaused()) {
				// Affichage optimisé
				pDispositif->Present();

				// On rend l'image sur la surface de travail
				// (tampon d'arrière plan)
				RenderScene(static_cast<float>(TempsEcoule));
			}

			// Calcul du temps du prochain affichage
			TempsCompteurPrecedent = TempsCompteurCourant;
		}
	}

	const DirectX::XMVECTOR GetPosition() const { return Position; }
	DirectX::XMMATRIX& GetMatView() { return m_MatView; }
	const DirectX::XMMATRIX& GetMatView() const { return m_MatView; }
	DirectX::XMMATRIX& GetMatProj() { return m_MatProj; }
	const DirectX::XMMATRIX& GetMatProj() const { return m_MatProj; }
	DirectX::XMMATRIX& GetMatViewProj() { return m_MatViewProj; }
	const DirectX::XMMATRIX& GetMatViewProj() const { return m_MatViewProj; }

	CDIManipulateur& GetGestionnaireDeSaisie() { return GestionnaireDeSaisie; }
	ResourcesManager& GetResourcesManager() { return ResourcesManager; }
	const Scene& GetScene() const noexcept { return *CurrentScene; }
	Scene& GetScene() noexcept { return *CurrentScene; }
	DeviceD3D11& GetDispositif() noexcept { return *pDispositif; }

	void Stop() noexcept { CurrentState |= EngineState::Stopping; }
	bool IsStopping() const noexcept { return IsStateSet(EngineState::Stopping); }

	void Pause() noexcept { CurrentState |= EngineState::Paused; }
	void UnPause() noexcept { CurrentState &= ~EngineState::Paused; }
	bool IsPaused() const noexcept { return IsStateSet(EngineState::Paused); }

protected:
	~CMoteur()
	{
		Cleanup();
	}

	// Spécifiques - Doivent être implantés
	virtual void RunSpecific() = 0;
	virtual int InitialisationsSpecific() = 0;

	virtual int64_t GetTimeSpecific() const = 0;
	virtual double GetTimeIntervalsInSec(int64_t start, int64_t stop) const = 0;

	virtual TDevice* CreationDispositifSpecific(const CDS_MODE cdsMode) = 0;
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

		// Tick the skybox at first
		CurrentScene->SkyBox->LateTick(ElapsedTime);

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
		delete CurrentScene;

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

		//// Initialisation des matrices View et Proj
		//// Dans notre cas, ces matrices sont fixes
		m_MatView = XMMatrixLookAtLH(XMVectorSet(0.0f, 2.0f, 10.0f, 1.0f), XMVectorSet(0.0f, 2.0f, 10.0f, 1.0f) + XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

		const float champDeVision = XM_PI / 2; 	// 45 degr�s
		const float ratioDAspect = static_cast<float>(pDispositif->ScreenWidth) / static_cast<float>(pDispositif->ScreenHeight);
		const float planRapproche = 1.0;
		const float planEloigne = 300.0;

		m_MatProj = XMMatrixPerspectiveFovLH(
			champDeVision,
			ratioDAspect,
			planRapproche,
			planEloigne);

		// Calcul de VP a l'avance
		m_MatViewProj = m_MatView * m_MatProj;

		// Create actors
		if (!InitObjets()) {
			return 1;
		}

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
		GestionnaireDeSaisie.StatutClavier();
		GestionnaireDeSaisie.SaisirEtatSouris();

		// Make all component Tick
		CurrentScene->Tick(tempsEcoule);

		// Update the scene to reflect components changes
		CurrentScene->Update();

		return true;
	}

protected:
	/// <summary>
	/// The current state of the engine, can have multiple values at a time.
	/// The values are defined via \ref EngineState flags.
	/// </summary>
	uint32_t CurrentState = EngineState::Starting;

	// Physic variable
	const float PhysicDeltaStep = 1.0f / 60.0f;
	float PhysicAccumulator = 0;
	int64_t CurrentPhysicTime;

	// Variables pour le temps de l'animation
	int64_t TempsSuivant;
	int64_t TempsCompteurPrecedent;

	// Le dispositif de rendu
	TDevice* pDispositif;

	// La seule scène
	Scene* CurrentScene;

	// Position of the camera
	DirectX::XMVECTOR Position;

	// Les matrices
	DirectX::XMMATRIX m_MatView;
	DirectX::XMMATRIX m_MatProj;
	DirectX::XMMATRIX m_MatViewProj;


	// Les saisies
	CDIManipulateur GestionnaireDeSaisie;
	ResourcesManager ResourcesManager;

private:
	/// <summary>
	/// Set a specific state flag to be true for the current state.
	/// </summary>
	void SetState(const EngineState& State) noexcept
	{
		CurrentState |= State;
	}
	/// <summary>
	/// Remove a specific state flag from the current state.
	/// </summary>
	void UnsetState(const EngineState& State) noexcept
	{
		CurrentState &= ~State;
	}

	/// <summary>
	/// Test if a given state is set in the current state.
	/// </summary>
	bool IsStateSet(const EngineState& State) const noexcept
	{
		return (CurrentState & State) == State;
	}
};

} // namespace PM3D


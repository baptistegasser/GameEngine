#include "stdafx.h"

#include "render/GameFactory.h"
#include "render/EngineD3D11.h"

// Physic components
#include "physic/RigidBody.h"
#include "physic/SphereCollider.h"
#include "physic/BoxCollider.h"
#include "physic/CapsuleCollider.h"

// Render components
#include "render/MeshRenderer.h"
#include "render/Camera.h"
#include "render/Terrain.h"
#include "render/SpotLight.h"
#include "render/PointLight.h"
#include "render/DirectionalLight.h"
#include "render/Skybox.h"
//#include "render/Font.h"

// Gameplay components
#include "Speed.h"
#include "gameplay/Plateform.h"
#include "gameplay/Player.h"
#include "gameplay/CheckPoint.h"
#include "gameplay/IntelligentEnemy.h"

void GameFactory::LoadLevel()
{
	ResourcesManager = &EngineD3D11::GetInstance().ResourcesManager;
	// Load the default shader !
	DefaultShader = ResourcesManager->GetShader(L".\\shaders\\MiniPhong.fx");

	EngineD3D11::GetInstance().EffectManager.AddEffect(L".\\shaders\\Effect_Nul.fx");
	EngineD3D11::GetInstance().EffectManager.ActivateEffect(wchar2str(L".\\shaders\\Effect_Nul.fx"));

	CreateTerrain(L".\\modeles\\heigtmap\\Arene.bmp", Math::Transform{ Math::Vec3f{ 0.f, -50.f, 0.f }, Math::Vec3f{ 2.f, 1.f, 2.f } });
	CreateEnemy(Math::Vec3f{ 0.f, -7.f, 1.f });
	CreatePlayer(Math::Vec3f(0, 10.5f, 0));
	CreateSkyBox(PlayerTransform);
	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 0), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");
	CreateMobilePlatform(Math::Vec3f(-15.f, 10.f, 25),  Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(15.f, 10.f, 35), Math::Vec3f(0, 0, -10), L".\\modeles\\plateform\\plateformSable.OMB");
	
	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 60.f), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateMobilePlatform(Math::Vec3f(-20.f, 10.f, 85), Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(0.f, 10.f, 105), Math::Vec3f(0, 0, -10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(20.f, 10.f, 85), Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");

	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 130), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateIntelligentEnemy(Math::Vec3f{ 0.f, 40.f, 1.f }, PlayerTransform, 40.0f);
	CreateCheckPoint(Math::Vec3f{ 0.f, 10.5f, 5.f });
	CreateCheckPoint(Math::Vec3f{ 0.f, 10.5f, 65.f });
	CreateCheckPoint(Math::Vec3f{ 0.f, 10.5f, 135.f });


	//CreateMobilePlatform(Math::Vec3f(10.f, 10.f, 20.f), Math::Vec3f(1.f, 1.f, 1.f), Math::Vec3f(-10, 0, 0), L".\\modeles\\plateform\\plateformGlace.OMB");
	CreateLights(DirectX::XMFLOAT3{ 0.f, 20.f, 0.f }, DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f }, DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f },  4.f);
}

void GameFactory::CreateTerrain(const wchar_t* Filename, Math::Transform Transform)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	const auto Terrain = new ATerrain{
			Filename,
			{1, 0.3f, 1},
			RessourceManager.GetShader(L".\\shaders\\MiniPhongTerrain.fx"),
			PhysicMaterial{ 0.5f, 0.5f, 0.2f}
		};
	Terrain->Texture1 = RessourceManager.GetTexture(L".\\modeles\\gazon.dds");
	Terrain->Texture2 = RessourceManager.GetTexture(L".\\modeles\\roche.dds");
	Terrain->Texture3 = RessourceManager.GetTexture(L".\\modeles\\chemin.dds");
	Terrain->Transform = Transform;
	Engine.GetScene().AddActor(Terrain, true);
}

void GameFactory::CreatePlayer(Math::Transform Transform)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto MyPlayer = new Pitbull::Actor{ "Player" };
	MyPlayer->AddComponent<MeshRenderer>(
		RessourceManager.GetMesh(L".\\modeles\\ball3\\ball.OMB"), 
		RessourceManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlayer->AddComponent<Player>(Transform.Position);
	MyPlayer->Transform = Transform;

	const auto PlayerCam = MyPlayer->AddComponent<Camera>(
		DirectX::XMVectorSet(0.0f, 2.0f, 10.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 0.4f, 1.0f, 1.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
		&Engine.MatView,
		&Engine.MatProj,
		&Engine.MatViewProj);

	Engine.GetScene().SetCurrentCamera(PlayerCam);
	MyPlayer->Transform.RotateX(-90.0f);

	MyPlayer->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 0.2f });
	const auto PlayerBody = MyPlayer->AddComponent<RigidBody>(RigidBody::RigidActorType::Dynamic);
	PlayerBody->SetMass(10.f);

	PlayerTransform = &MyPlayer->Transform;

	const auto Text = MyPlayer->AddComponent<TextRenderer>(
		new Font{ L"Arial", Gdiplus::FontStyleBold, 32.0f, { 0, 0, 0} },
		RessourceManager.GetShaderSprite(L".\\shaders\\sprite1.fx"),
		100, 500);
	Text->Offset.Position.y = 0.7f;
	Text->Offset.Position.x = -0.5f;
	MyPlayer->AddComponent<Speed>();
	
	Engine.GetScene().AddActor(MyPlayer);
}

void GameFactory::CreateEnemy(Math::Transform Transform)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto Ennemy = new Pitbull::Actor{};
	Ennemy->AddComponent<MeshRenderer>(
		RessourceManager.GetMesh(L".\\modeles\\cube\\cube.OMB"), 
		RessourceManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	Ennemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f });
	Ennemy->Transform = Transform;

	auto body = Ennemy->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	Ennemy->AddComponent<Plateform>(
		Math::Transform(Ennemy->Transform.Position, Math::Quaternion(-physx::PxHalfPi, Math::Vec3f(0, 1, 0)))
		, Math::Transform(Ennemy->Transform.Position + Math::Vec3f(10, 0, 0), Math::Quaternion(physx::PxHalfPi, Math::Vec3f(0, 1, 0))), true);

	const auto Hat = Ennemy->AddComponent<SpriteRenderer>(
		RessourceManager.GetTexture(L".\\modeles\\hat.dds"), 
		RessourceManager.GetShaderSprite(L".\\shaders\\sprite1.fx"), true);
	Hat->Offset.Position.y = 1.9f;
	Hat->Offset.Scale.x = 4.0f;
	Hat->Offset.Scale.y = 4.0f;

	Engine.GetScene().AddActor(Ennemy);
}

void GameFactory::CreateIntelligentEnemy(Math::Transform Transform, Math::Transform* ToFollow, float Distance)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto Ennemy = new Pitbull::Actor{};
	Ennemy->AddComponent<MeshRenderer>(
		RessourceManager.GetMesh(L".\\modeles\\cube\\cube.OMB"),
		RessourceManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	Ennemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f });
	Ennemy->Transform = Transform;
	Ennemy->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	Ennemy->AddComponent<IntelligentEnemy>(ToFollow, Distance);
	Engine.GetScene().AddActor(Ennemy);
}

void GameFactory::CreatePlatform(Math::Transform Transform, const wchar_t* Filename, PhysicMaterial Material)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto MyPlateform = new Pitbull::Actor{};
	MyPlateform->AddComponent<MeshRenderer>(RessourceManager.GetMesh(Filename),
		RessourceManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlateform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlateform->Transform = Transform;
	Engine.GetScene().AddActor(MyPlateform);
}

void GameFactory::CreateMobilePlatform(Math::Transform Transform, Math::Vec3f End, const wchar_t* Filename, PhysicMaterial Material)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto MyPlateform = new Pitbull::Actor{};
	MyPlateform->AddComponent<MeshRenderer>(
		RessourceManager.GetMesh(Filename),
		RessourceManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlateform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlateform->Transform = Transform;
	MyPlateform->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	MyPlateform->AddComponent<Plateform>(
		Math::Transform(MyPlateform->Transform.Position)
		, Math::Transform(MyPlateform->Transform.Position + End), true);
	Engine.GetScene().AddActor(MyPlateform);
}

void GameFactory::CreateLights(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT3 Specular, DirectX::XMFLOAT3 Roughness, float Intensity, float InnerRadius, float OuterRadius)
{
	auto& CurrentScene = EngineD3D11::GetInstance().GetScene();

	CurrentScene.LightManager.AmbientColor = { 0.5f };

	auto DirLight = new ADirectionalLight;
	DirLight->GetLight()->Direction = { 0.f, 4.f, 0.f };
	DirLight->GetLight()->Color = { 0.f, 1.f, 1.f };
	CurrentScene.AddActor(DirLight);

	auto ALightRed = new APointLight;
	auto LightRed = ALightRed->GetLight();
	LightRed->Position = { 0.f, 5.f, 0.f };
	LightRed->Color = { 1.0f, 0.0f, 0.0f };
	LightRed->Range = 20.f;
	CurrentScene.AddActor(ALightRed);

	auto ALightBlue = new APointLight;
	auto LightBlue = ALightBlue->GetLight();
	LightBlue->Position = { -40.f, 5.f, 0.f };
	LightBlue->Color = { 0.f, 0.0f, 1.0f };
	LightBlue->Range = 20.f;
	CurrentScene.AddActor(ALightBlue);

	auto ALightGreen = new APointLight;
	auto LightGreen = ALightGreen->GetLight();
	LightGreen->Position = { 40.f, 5.f, 0.f };
	LightGreen->Color = { 0.0f, 1.0f, 0.0f };
	LightGreen->Range = 20.f;
	CurrentScene.AddActor(ALightGreen);

	auto ASpotRed = new ASpotLight;
	auto SpotRed = ASpotRed->GetLight();
	SpotRed->Position = { 0.f, 5.f, -40.f };
	SpotRed->Direction = { -30.f, 0.f, -40.f };
	SpotRed->Color = { 1.0f, 0.0f, 0.0f };
	SpotRed->Range = 20.f;
	CurrentScene.AddActor(ASpotRed);
}

void GameFactory::CreateSkyBox(Math::Transform* ToFollow)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	Engine.GetScene().AddSkyBox(new Skybox{
			ToFollow
			, RessourceManager.GetMesh(L".\\modeles\\sky\\sky.OMB")
			, RessourceManager.GetShader(L".\\shaders\\MiniPhongSkyBox.fx")
		}
	);
}


void GameFactory::CreateCheckPoint(Math::Transform Transform)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto MyCheckPoint = new Pitbull::Actor{ "CheckPoint" };
	MyCheckPoint->AddComponent<MeshRenderer>(
		RessourceManager.GetMesh(L".\\modeles\\checkPoint\\star.OMB"),
		RessourceManager.GetShader(L".\\shaders\\MiniPhong.fx"));

	MyCheckPoint->Transform = Transform;
	MyCheckPoint->AddComponent<CheckPoint>();

	auto CheckPointCollider = [&RessourceManager](const Contact& Contact) -> void {
		if (Contact.FirstActor->Name == "CheckPoint" && Contact.SecondActor->Name == "Player" && !Contact.FirstActor->GetComponent<CheckPoint>()->IsVisited)
		{
			Contact.SecondActor->GetComponent<Player>()->SetSpawnPos(Contact.FirstActor->Transform.Position);
			Contact.FirstActor->GetComponent<CheckPoint>()->IsVisited = true;
			Contact.FirstActor->GetComponent<MeshRenderer>()->Mesh = RessourceManager.GetMesh(L".\\modeles\\checkPoint\\green_star.OMB");
		}
		else if (Contact.FirstActor->Name == "Player" && Contact.SecondActor->Name == "CheckPoint" && !Contact.SecondActor->GetComponent<CheckPoint>()->IsVisited)
		{
			Contact.FirstActor->GetComponent<Player>()->SetSpawnPos(Contact.SecondActor->Transform.Position);
			Contact.SecondActor->GetComponent<CheckPoint>()->IsVisited = true;
			Contact.SecondActor->GetComponent<MeshRenderer>()->Mesh = RessourceManager.GetMesh(L".\\modeles\\checkPoint\\green_star.OMB");
		}
	};

	auto Collider = MyCheckPoint->AddComponent<CapsuleCollider>(0.4f, 0.01f, PhysicMaterial{ 0.5f, 0.5f, 0.2f });
	Collider->OnContactCallBack = CheckPointCollider;

	MyCheckPoint->AddComponent<RigidBody>(RigidBody::RigidActorType::Static, true);

	Engine.GetScene().AddActor(MyCheckPoint);
}

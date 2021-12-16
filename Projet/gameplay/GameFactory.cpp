#include "stdafx.h"

#include "gameplay/GameFactory.h"
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
#include "render/Speed.h"
#include "render/Timer.h"
#include "gameplay/Plateform.h"
#include "gameplay/Player.h"
#include "gameplay/CheckPoint.h"

#include <math.h>
#include "math/Math.h"

void GameFactory::LoadLevel()
{
	/***
	 * Util Object
	 ***/
	CreatePlayer(Math::Vec3f(0, 10.5f, 0));
	CreateSkyBox(PlayerTransform);
	CreateLights(DirectX::XMFLOAT3{ 0.f, 20.f, 0.f }, DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f }, DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f }, 4.f);
	CreateTimer();

	/***
	 * First state with platform ( z C 0,135)
	 ***/
	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 0), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateMobilePlatform(Math::Vec3f(-15.f, 10.f, 25),  Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(-15.f, 10.f, 25),  Math::Vec3f(0, 0, 10), L".\\modeles\\tree_cloud\\tree_cloud.OMB");
	CreateMobilePlatform(Math::Vec3f(15.f, 10.f, 35), Math::Vec3f(0, 0, -10), L".\\modeles\\plateform\\plateformSable.OMB");
	
	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 60.f), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateMobilePlatform(Math::Vec3f(-20.f, 10.f, 85), Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(0.f, 10.f, 105), Math::Vec3f(0, 0, -10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(20.f, 10.f, 85), Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");

	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 130), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateCheckPoint(Math::Transform{ Math::Vec3f{ 0.f, 10.5f, 135.f }, Math::Vec3f{ 1.5f, 1.5f, 1.5f } });

	/***
	 * First State with arena ( z C 135,210)
	 ***/

	Math::Vec3f TerrainPos = { -38.f, 3.f, 145.f };
	float PosYEnemy = 4.3f;

	// Dim : 76 / 76
	auto GrassTerrain = CreateTerrain(L".\\modeles\\heigtmap\\Arene.bmp",
		Math::Transform{ TerrainPos, Math::Vec3f{ 0.15f, 0.15f, 0.15f }, Math::Quaternion{ 0, Math::Vec3f(0, 1, 0)} },
		L".\\modeles\\gazon.dds", L".\\modeles\\terre.dds", L".\\modeles\\Arene_Texture_2.dds", true);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 5.f, PosYEnemy, TerrainPos.z + 10.f }, Math::Vec3f{ TerrainPos.x + 71.f, PosYEnemy, TerrainPos.z + 10.f }, true);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 5.f, PosYEnemy, TerrainPos.z + 20.f }, Math::Vec3f{ TerrainPos.x + 34.f, PosYEnemy, TerrainPos.z + 20.f }, true);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 73.f,PosYEnemy, TerrainPos.z + 20.f }, Math::Vec3f{ TerrainPos.x + 44.f, PosYEnemy, TerrainPos.z + 20.f }, true);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 45.f, PosYEnemy, TerrainPos.z + 40.f }, Math::Vec3f{ TerrainPos.x + 45.f, PosYEnemy, TerrainPos.z + 25.f }, true, 0.2f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 35.f,PosYEnemy, TerrainPos.z + 40.f }, Math::Vec3f{ TerrainPos.x + 35.f,PosYEnemy, TerrainPos.z + 25.f }, true, 0.2f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 45.f, PosYEnemy, TerrainPos.z + 60.f }, Math::Vec3f{ TerrainPos.x + 45.f, PosYEnemy, TerrainPos.z + 45.f }, true, 0.2f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 35.f,PosYEnemy, TerrainPos.z + 60.f }, Math::Vec3f{ TerrainPos.x + 35.f,PosYEnemy, TerrainPos.z + 45.f }, true, 0.2f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 5.f,PosYEnemy, TerrainPos.z + 42.5f }, Math::Vec3f{ TerrainPos.x + 44.f,PosYEnemy, TerrainPos.z + 42.5f }, true, 0.4f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 34.f,PosYEnemy, TerrainPos.z + 42.5f }, Math::Vec3f{ TerrainPos.x + 73.f,PosYEnemy, TerrainPos.z + 42.5f }, true, 0.4f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 44.f,PosYEnemy, TerrainPos.z + 62.5f }, Math::Vec3f{ TerrainPos.x + 5.f,PosYEnemy, TerrainPos.z + 62.5f }, true, 0.4f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 73.f,PosYEnemy, TerrainPos.z + 62.5f }, Math::Vec3f{ TerrainPos.x + 34.f,PosYEnemy, TerrainPos.z + 62.5f }, true, 0.4f);
	CreateIntelligentEnemy(Math::Transform{ Math::Vec3f{ TerrainPos.x + 15.f, PosYEnemy, TerrainPos.z + 35.f }, Math::Vec3f(0.1f, 0.1f, 0.1f) }, PlayerTransform,
		IntelligentEnemy::ActionZone{ TerrainPos, Math::Vec3f{ TerrainPos.x + 76.f, PosYEnemy + 30.f, TerrainPos.z + 76.f } }, GrassTerrain, 
		Math::Vec3f{ 15.f, PosYEnemy, 35.f }, 40.0f);
	CreateIntelligentEnemy(Math::Transform{ Math::Vec3f{ TerrainPos.x + 63.f, PosYEnemy, TerrainPos.z + 35.f }, Math::Vec3f(0.1f, 0.1f, 0.1f) }, PlayerTransform,
		IntelligentEnemy::ActionZone{ TerrainPos, Math::Vec3f{ TerrainPos.x + 76.f, PosYEnemy + 30.f, TerrainPos.z + 76.f } }, GrassTerrain, 
		Math::Vec3f{ 63.f, PosYEnemy, 35.f }, 40.0f);

	/***
	 * Second state with plateform
	 ***/
	
	Math::Vec3f BasePos = { 0.f, -3.f, 240.f };

	CreatePlatform(Math::Transform{ BasePos, Math::Vec3f{ 2.f, 1.f, 1.5f } }, L".\\modeles\\plateform\\plateformGlace.OMB");
	CreateCheckPoint(Math::Transform{ Math::Vec3f{ BasePos.x, BasePos.y + 0.5f, BasePos.z }, Math::Vec3f{ 1.5f, 1.5f, 1.5f } });

	CreateMobilePlatform(Math::Vec3f(BasePos.x, BasePos.y, BasePos.z + 20.f), Math::Vec3f(-55.f, 0, 0), 
		L".\\modeles\\plateform\\plateformSable.OMB", Collider::DefaultMaterial,  0.175f);
	CreatePlatform(Math::Transform{ Math::Vec3f(BasePos.x - 60.f, BasePos.y, BasePos.z + 40.f), Math::Vec3f{ 1.f, 1.f, 1.f } }, 
		L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateCheckPoint(Math::Transform{ Math::Vec3f{ BasePos.x - 60.f, BasePos.y + 0.5f, BasePos.z + 40.f }, Math::Vec3f{ 1.5f, 1.5f, 1.5f } });

	CreateMobilePlatform(Math::Transform{ Math::Vec3f(BasePos.x - 60.f, BasePos.y, BasePos.z + 55.f), Math::Vec3f{ 0.75f, 1.f, 0.75f } }, 
		Math::Vec3f(0, 0, 20), L".\\modeles\\plateform\\plateformSable.OMB", Collider::DefaultMaterial, 0.15f);
	CreateMobilePlatform(Math::Transform{ Math::Vec3f(BasePos.x - 95.f, BasePos.y, BasePos.z + 90.f), Math::Vec3f{ 0.75f, 1.f, 0.75f } },
		Math::Vec3f(20, 0, -20), L".\\modeles\\plateform\\plateformSable.OMB", Collider::DefaultMaterial, 0.20f);

	CreatePlatform(Math::Transform{ Math::Vec3f(BasePos.x - 95.f, BasePos.y, BasePos.z + 115.f), Math::Vec3f{ 1.75f, 1.f, 1.75f } }, 
		L".\\modeles\\plateform\\plateformRouge.OMB");
	CreateCheckPoint(Math::Transform{ Math::Vec3f{ BasePos.x - 95.f, BasePos.y + 0.5f, BasePos.z + 115.f }, Math::Vec3f{ 1.5f, 1.5f, 1.5f } });


	/***
	 * Second State with arena
	 ***/
	Math::Vec3f TerrainPos2 = { -115.f, -6.f, 375.f };
	float PosYEnemy2 = -5.5f;

	// Dim : 128 / 128
	CreateTerrain(L".\\modeles\\heigtmap\\ground.bmp",
		Math::Transform{ TerrainPos2, Math::Vec3f{ 0.25f, 0.15f, 0.25f } },
		L".\\modeles\\mur_pierre.dds",  L".\\modeles\\terre.dds", L".\\modeles\\Arene_Texture_2.dds", true);
	Math::Transform CaveTrans = Math::Transform{ Math::Vec3f{ TerrainPos2.x + 128.f, TerrainPos2.y + 22.0f, TerrainPos2.z }, Math::Vec3f{ 0.25f, 0.15f, 0.25f } };
	CaveTrans.RotateZ(180);
	CreateTerrain(L".\\modeles\\heigtmap\\ground_reverse.bmp",
		CaveTrans,
		L".\\modeles\\mur_pierre.dds", L".\\modeles\\terre.dds", L".\\modeles\\Arene_Texture_2.dds", true);

	/***
	 * Third state with plateform
	 ***/

	Math::Vec3f BasePos2 = { -10.f, -6.f, 515.f };

	CreatePlatform(Math::Transform{ BasePos2, Math::Vec3f{ 2.f, 1.f, 1.5f } }, L".\\modeles\\plateform\\plateformRouge.OMB");
	CreateCheckPoint(Math::Transform{ Math::Vec3f{ BasePos2.x, BasePos2.y + 0.5f, BasePos2.z }, Math::Vec3f{ 1.5f, 1.5f, 1.5f } });

	CreateMobilePlatform(Math::Transform{ Math::Vec3f(BasePos2.x, BasePos2.y, BasePos2.z + 20.f), Math::Vec3f{ 0.75f, 0.75f, 0.75f } }, Math::Vec3f(0.f, 20.f, 0),
		L".\\modeles\\plateform\\plateformSable.OMB", Collider::DefaultMaterial);
	CreateEnemy(Math::Vec3f{ BasePos2.x - 15.f, BasePos2.y + 7.f, BasePos2.z + 20.f }, Math::Vec3f{ BasePos2.x + 15.f, BasePos2.y + 7.f, BasePos2.z + 20.f }, true);
	CreateEnemy(Math::Vec3f{ BasePos2.x - 15.f, BasePos2.y + 13.f, BasePos2.z + 20.f }, Math::Vec3f{ BasePos2.x + 15.f, BasePos2.y + 13.f, BasePos2.z + 20.f }, true);

	CreatePlatform(Math::Transform{ Math::Vec3f{ BasePos2.x, BasePos2.y + 20.f, BasePos2.z + 40.f }, Math::Vec3f{ 2.f, 1.f, 1.5f } }, L".\\modeles\\plateform\\plateformRouge.OMB");
	CreateCheckPoint(Math::Transform{ Math::Vec3f{ BasePos2.x, BasePos2.y + 20.5f, BasePos2.z +40.f }, Math::Vec3f{ 1.5f, 1.5f, 1.5f } });



	/***
	 * Third State with arena
	 ***/
	Math::Vec3f TerrainPos3 = { -96.f, 0.f, 570.f };
	//float PosYEnemy2 = -5.5f;

	// Dim : 128 / 128
	CreateTerrain(L".\\modeles\\heigtmap\\fall.bmp",
		Math::Transform{ TerrainPos3, Math::Vec3f{ 0.25f, 0.15f, 0.25f } },
		L".\\modeles\\ice2.dds", L".\\modeles\\sand.dds", L".\\modeles\\ways.dds", true);
	
	/***
	* Finish
	***/
	Math::Transform TunnelTrans = Math::Transform{ Math::Vec3f(TerrainPos3.x + 20.f, TerrainPos3.y + 9.f, TerrainPos3.z + 130.f), Math::Vec3f(0.5f, 0.05f, 0.07f) };

	TunnelTrans.RotateY(270);
	TunnelTrans.RotateZ(335);

	CreateSlide(TunnelTrans);
	
	CreatePlatform(Math::Transform{ Math::Vec3f{ TerrainPos3.x + 16.f, TerrainPos3.y - 20.f, TerrainPos3.z + 220.f }, Math::Vec3f{ 2.f, 1.f, 5.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateGoal(Math::Transform{ Math::Vec3f(TerrainPos3.x + 16.f, TerrainPos3.y - 19.f, TerrainPos3.z + 250.f), Math::Vec3f(5.f, 5.f, 5.f) }, L".\\modeles\\tree_cloud\\tree_cloud.OMB");
}

ATerrain* GameFactory::CreateTerrain(const wchar_t* Filename, Math::Transform Transform, const std::wstring& TextureName1, const std::wstring& TextureName2, const std::wstring& TextureName3, bool FaceCull)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	const auto Terrain = new ATerrain{
			Filename,
			{1, 0.3f, 1},
			RessourceManager.GetShaderTerrain(L".\\shaders\\MiniPhongTerrain.fx"),
			PhysicMaterial{ 0.5f, 0.5f, 0.2f}
			, FaceCull
		};
	Terrain->Texture1 = RessourceManager.GetTexture(TextureName1);
	Terrain->Texture2 = RessourceManager.GetTexture(TextureName2);
	Terrain->Texture3 = RessourceManager.GetTexture(TextureName3);
	Terrain->Transform = Transform;
	Engine.GetScene().AddActor(Terrain, true);
	return Terrain;
}

void GameFactory::CreatePlayer(Math::Transform Transform)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto MyPlayer = new Pitbull::Actor{ "Player" };
	MyPlayer->AddComponent<MeshRenderer>(RessourceManager.GetMesh(L".\\modeles\\ball\\ball.OMB"),
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

void GameFactory::CreateEnemy(Math::Transform Transform, Math::Vec3f End, bool IsKiller, float Speed)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto Ennemy = new Pitbull::Actor{ "Enemy" };
	Ennemy->AddComponent<MeshRenderer>(RessourceManager.GetMesh(L".\\modeles\\bird\\bird.OMB"),
		RessourceManager.GetShader(L".\\shaders\\MiniPhong.fx"));

	Ennemy->AddComponent<Enemy>(IsKiller);

	auto EnemyCollider = [](const Contact& Contact) -> void {
		if (Contact.FirstActor->Name == "Enemy" && Contact.SecondActor->Name == "Player" && Contact.FirstActor->GetComponent<Enemy>()->IsKiller)
		{
			Contact.SecondActor->GetComponent<Player>()->RespawnPlayer();
		}
		else if (Contact.FirstActor->Name == "Player" && Contact.SecondActor->Name == "Enemy" && Contact.SecondActor->GetComponent<Enemy>()->IsKiller)
		{
			Contact.SecondActor->GetComponent<Player>()->RespawnPlayer();
		}
	};

	auto Collider = Ennemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f });
	Collider->OnContactCallBack = EnemyCollider;

	Ennemy->Transform = Transform;

	auto body = Ennemy->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	auto Movement = Ennemy->AddComponent<Plateform>(
		Math::Transform(Ennemy->Transform.Position, Math::Quaternion(-physx::PxHalfPi, Math::Vec3f(0, 1, 0)))
		, Math::Transform(End, Math::Quaternion(physx::PxHalfPi, Math::Vec3f(0, 1, 0))), true);
	Movement->SetSpeed(Speed);
	Engine.GetScene().AddActor(std::move(Ennemy));
}

void GameFactory::CreateIntelligentEnemy(Math::Transform Transform, Math::Transform* ToFollow, IntelligentEnemy::ActionZone Zone, ATerrain* RelativeTerrain,
	Math::Vec3f RelativeTerrainPosition, float Distance, bool IsKiller, float Speed)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto Ennemy = new Pitbull::Actor{};
	Ennemy->AddComponent<MeshRenderer>(
		RessourceManager.GetMesh(L".\\modeles\\bear\\bear.OMB"),
		RessourceManager.GetShader(L".\\shaders\\MiniPhong.fx"));
	Ennemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f });
	Ennemy->Transform = Transform;
	Ennemy->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	Ennemy->AddComponent<IntelligentEnemy>(ToFollow, Zone, Transform.Position, RelativeTerrain, RelativeTerrainPosition, Distance, IsKiller);
	Engine.GetScene().AddActor(std::move(Ennemy));
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

void GameFactory::CreateMobilePlatform(Math::Transform Transform, Math::Vec3f End, const wchar_t* Filename, PhysicMaterial Material, float Speed)
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
	auto Movement = MyPlateform->AddComponent<Plateform>(
		Math::Transform(MyPlateform->Transform.Position)
		, Math::Transform(MyPlateform->Transform.Position + End), true);
	Movement->SetSpeed(Speed);

	Engine.GetScene().AddActor(std::move(MyPlateform));
}

void GameFactory::CreateLights(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT3 Specular, DirectX::XMFLOAT3 Roughness, float Intensity, float InnerRadius, float OuterRadius)
{
	auto& CurrentScene = EngineD3D11::GetInstance().GetScene();

	CurrentScene.LightManager.AmbientColor = { 0.7f };

	auto DirLight = new ADirectionalLight;
	DirLight->GetLight()->Direction = { -0.5f, -1.f, 0.f };
	DirLight->GetLight()->Color = { 0.5f, 0.5f, 0.5f };
	CurrentScene.AddActor(DirLight);

	auto ALightRed = new APointLight;
	auto LightRed = ALightRed->GetLight();
	LightRed->Position = { 0.f, 5.f, 0.f };
	LightRed->Color = { 1.0f, 1.0f, 1.0f };
	LightRed->Range = 20.f;
	CurrentScene.AddActor(ALightRed);

	auto ALightBlue = new APointLight;
	auto LightBlue = ALightBlue->GetLight();
	LightBlue->Position = { -40.f, 5.f, 0.f };
	LightBlue->Color = { 1.0f, 1.0f, 1.0f };
	LightBlue->Range = 20.f;
	CurrentScene.AddActor(ALightBlue);

	auto ALightGreen = new APointLight;
	auto LightGreen = ALightGreen->GetLight();
	LightGreen->Position = { 40.f, 5.f, 0.f };
	LightGreen->Color = { 1.0f, 1.0f, 1.0f };
	LightGreen->Range = 20.f;
	CurrentScene.AddActor(ALightGreen);

	auto ASpotRed = new ASpotLight;
	auto SpotRed = ASpotRed->GetLight();
	SpotRed->Position = { 0.f, 5.f, -40.f };
	SpotRed->Direction = { -30.f, 0.f, -40.f };
	SpotRed->Color = { 1.0f, 1.0f, 1.0f };
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

void GameFactory::CreateSlide(Math::Transform Transform)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	const auto Terrain = new ATerrain{
			L".\\modeles\\heigtmap\\tunnel.bmp",
			{1, 0.3f, 1},
			RessourceManager.GetShaderTerrain(L".\\shaders\\MiniPhongTerrain.fx"),
			PhysicMaterial{ 0.5f, 0.5f, 0.2f}
			, true
		};
	Terrain->Texture1 = RessourceManager.GetTexture(L".\\modeles\\tunnel\\dark_blue.dds");
	Terrain->Texture2 = RessourceManager.GetTexture(L".\\modeles\\tunnel\\rouge.dds");
	Terrain->Texture3 = RessourceManager.GetTexture(L".\\modeles\\tunnel\\zebra3.dds");
	Terrain->Transform = Transform;

	Engine.GetScene().AddActor(Terrain, true);
}

void GameFactory::CreateGoal(Math::Transform Transform, const wchar_t* Filename)
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto MyGoal = new Pitbull::Actor{ "Goal" };

	MyGoal->AddComponent<SpriteRenderer>(RessourceManager.GetTexture(L".\\modeles\\Goal.dds"),RessourceManager.GetShaderSprite(L".\\shaders\\sprite1.fx"), true);

	auto GoalCollider = [](const Contact& Contact) -> void {
		if (Contact.FirstActor->Name == "Goal" && Contact.SecondActor->Name == "Player")
		{
			std::cout << "Fini la course";
		}
		else if (Contact.FirstActor->Name == "Player" && Contact.SecondActor->Name == "Goal")
		{
			std::cout << "Fini la course";
		}
	};

	auto Collider = MyGoal->AddComponent<CapsuleCollider>(0.05f, 0.05f, PhysicMaterial{ 0.5f, 0.5f, 0.2f });
	Collider->OnContactCallBack = GoalCollider;
	MyGoal->Transform = Transform;

	Engine.GetScene().AddActor(std::move(MyGoal));
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

void GameFactory::CreateTimer()
{
	auto& Engine = EngineD3D11::GetInstance();
	auto& RessourceManager = Engine.ResourcesManager;

	auto MyTimer = new Pitbull::Actor{};
	const auto Text = MyTimer->AddComponent<TextRenderer>(
		new Font{ L"Arial", Gdiplus::FontStyleBold, 32.0f, { 0, 0, 0} },
		RessourceManager.GetShaderSprite(L".\\shaders\\sprite1.fx"),
		100, 500);
	Text->Offset.Position.y = 0.7f;
	Text->Offset.Position.x = 0.3f;
	MyTimer->AddComponent<Timer>();

	Engine.GetScene().AddActor(MyTimer, true);
}

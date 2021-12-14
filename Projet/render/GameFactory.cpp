#include "stdafx.h"

#include "render/GameFactory.h"
#include "render/MoteurWindows.h"

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

// Gameplay components
#include "gameplay/Plateform.h"
#include "gameplay/Player.h"
#include "gameplay/CheckPoint.h"
#include "gameplay/IntelligentEnemy.h"

void GameFactory::LoadLevel()
{
	/***
	 * Util Object
	 ***/
	CreatePlayer(Math::Vec3f(0, 10.5f, 0));
	CreateSkyBox(PlayerTransform);
	CreateLights(DirectX::XMFLOAT3{ 0.f, 20.f, 0.f }, DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f }, DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f }, 4.f);

	/***
	 * First state with platform ( z C 0,135)
	 ***/
	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 0), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateMobilePlatform(Math::Vec3f(-15.f, 10.f, 25),  Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(15.f, 10.f, 35), Math::Vec3f(0, 0, -10), L".\\modeles\\plateform\\plateformSable.OMB");
	
	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 60.f), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateMobilePlatform(Math::Vec3f(-20.f, 10.f, 85), Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(0.f, 10.f, 105), Math::Vec3f(0, 0, -10), L".\\modeles\\plateform\\plateformSable.OMB");
	CreateMobilePlatform(Math::Vec3f(20.f, 10.f, 85), Math::Vec3f(0, 0, 10), L".\\modeles\\plateform\\plateformSable.OMB");

	CreatePlatform(Math::Transform{ Math::Vec3f(0, 10.f, 130), Math::Vec3f{ 5.f, 1.f, 2.f } }, L".\\modeles\\plateform\\plateformRouge.OMB");

	CreateCheckPoint(Math::Vec3f{ 0.f, 10.5f, 135.f });

	/***
	 * First State with arena ( z C 135,210)
	 ***/
	Math::Vec3f TerrainPos = { -38.f, 3.f, 145.f };
	float PosYEnemie = 4.3f;

	CreateTerrain(L".\\modeles\\heigtmap\\Arene.bmp",
		Math::Transform{ TerrainPos, Math::Vec3f{ 0.15f, 0.15f, 0.15f }, Math::Quaternion{ 0, Math::Vec3f(0, 1, 0)} });
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 5.f, PosYEnemie, TerrainPos.z + 10.f }, Math::Vec3f{ TerrainPos.x + 71.f, PosYEnemie, TerrainPos.z + 10.f }, true);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 5.f, PosYEnemie, TerrainPos.z + 20.f }, Math::Vec3f{ TerrainPos.x + 34.f, PosYEnemie, TerrainPos.z + 20.f }, true);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 73.f,PosYEnemie, TerrainPos.z + 20.f }, Math::Vec3f{ TerrainPos.x + 44.f, PosYEnemie, TerrainPos.z + 20.f }, true);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 45.f, PosYEnemie, TerrainPos.z + 40.f }, Math::Vec3f{ TerrainPos.x + 45.f, PosYEnemie, TerrainPos.z + 25.f }, true, 0.2f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 35.f,PosYEnemie, TerrainPos.z + 40.f }, Math::Vec3f{ TerrainPos.x + 35.f,PosYEnemie, TerrainPos.z + 25.f }, true, 0.2f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 45.f, PosYEnemie, TerrainPos.z + 60.f }, Math::Vec3f{ TerrainPos.x + 45.f, PosYEnemie, TerrainPos.z + 45.f }, true, 0.2f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 35.f,PosYEnemie, TerrainPos.z + 60.f }, Math::Vec3f{ TerrainPos.x + 35.f,PosYEnemie, TerrainPos.z + 45.f }, true, 0.2f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 5.f,PosYEnemie, TerrainPos.z + 42.5f }, Math::Vec3f{ TerrainPos.x + 44.f,PosYEnemie, TerrainPos.z + 42.5f }, true, 0.4f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 34.f,PosYEnemie, TerrainPos.z + 42.5f }, Math::Vec3f{ TerrainPos.x + 73.f,PosYEnemie, TerrainPos.z + 42.5f }, true, 0.4f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 44.f,PosYEnemie, TerrainPos.z + 62.5f }, Math::Vec3f{ TerrainPos.x + 5.f,PosYEnemie, TerrainPos.z + 62.5f }, true, 0.4f);
	CreateEnemy(Math::Vec3f{ TerrainPos.x + 73.f,PosYEnemie, TerrainPos.z + 62.5f }, Math::Vec3f{ TerrainPos.x + 34.f,PosYEnemie, TerrainPos.z + 62.5f }, true, 0.4f);

	/***
	 * Second state with plateform
	 ***/

	CreatePlatform(Math::Transform{ Math::Vec3f(0, -3.f, 240), Math::Vec3f{ 2.f, 1.f, 1.5f } }, L".\\modeles\\plateform\\plateformRouge.OMB");
	CreateCheckPoint(Math::Vec3f{ 0.f, -2.5f, 240.f });

	//CreateIntelligentEnemy(Math::Vec3f{ 0.f, 40.f, 1.f }, PlayerTransform, 40.0f);

	//CreateMobilePlatform(Math::Vec3f(10.f, 10.f, 20.f), Math::Vec3f(1.f, 1.f, 1.f), Math::Vec3f(-10, 0, 0), L".\\modeles\\plateform\\plateformGlace.OMB");
	
}

void GameFactory::CreateTerrain(const wchar_t* Filename, Math::Transform Transform)
{
	auto Terrain = std::unique_ptr<ATerrain>(new ATerrain{
			Filename,
			{1, 0.3f, 1},
			PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhongTerrain.fx"),
			PhysicMaterial{ 0.5f, 0.5f, 0.2f}
		});
	Terrain->Texture1 = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetTexture(L".\\modeles\\gazon.dds");
	Terrain->Texture2 = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetTexture(L".\\modeles\\roche.dds");
	Terrain->Texture3 = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetTexture(L".\\modeles\\chemin.dds");
	Terrain->Transform = Transform;
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(Terrain), true);
}

void GameFactory::CreatePlayer(Math::Transform Transform)
{
	auto MyPlayer = Pitbull::Actor::New("Player");
	MyPlayer->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\ball3\\ball.OMB"), 
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlayer->AddComponent<Player>(Transform.Position);
	MyPlayer->Transform = Transform;

	auto PlayerCam = MyPlayer->AddComponent<Camera>(DirectX::XMVectorSet(0.0f, 2.0f, 10.0f, 1.0f),
	                                                DirectX::XMVectorSet(0.0f, 0.4f, 1.0f, 1.0f),
	                                                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
	                                                &PM3D::CMoteurWindows::GetInstance().GetMatView(),
	                                                &PM3D::CMoteurWindows::GetInstance().GetMatProj(),
	                                                &PM3D::CMoteurWindows::GetInstance().GetMatViewProj());

	PM3D::CMoteurWindows::GetInstance().GetScene().SetCurrentCamera(PlayerCam);
	MyPlayer->Transform.RotateX(-90.0f);

	MyPlayer->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 0.2f });
	auto PlayerBody = MyPlayer->AddComponent<RigidBody>(RigidBody::RigidActorType::Dynamic);
	PlayerBody->SetMass(10.f);

	PlayerTransform = &MyPlayer->Transform;

	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyPlayer));


}

void GameFactory::CreateEnemy(Math::Transform Transform, Math::Vec3f End, bool IsKiller, float Speed)
{
	auto Ennemy = Pitbull::Actor::New("Enemy");
	Ennemy->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\cube\\cube.OMB"), 
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));

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
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(Ennemy));
}

void GameFactory::CreateIntelligentEnemy(Math::Transform Transform, Math::Transform* ToFollow, float Distance, bool IsKiller, float Speed)
{
	auto Ennemy = Pitbull::Actor::New();
	Ennemy->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\cube\\cube.OMB"),
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	Ennemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f });
	Ennemy->Transform = Transform;
	Ennemy->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	Ennemy->AddComponent<IntelligentEnemy>(ToFollow, Distance, IsKiller);
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(Ennemy));
}

void GameFactory::CreatePlatform(Math::Transform Transform, const wchar_t* Filename, PhysicMaterial Material)
{
	auto MyPlateform = Pitbull::Actor::New();
	MyPlateform->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(Filename),
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlateform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlateform->Transform = Transform;
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyPlateform));
}

void GameFactory::CreateMobilePlatform(Math::Transform Transform, Math::Vec3f End, const wchar_t* Filename, PhysicMaterial Material, float Speed)
{
	auto MyPlateform = Pitbull::Actor::New();
	MyPlateform->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(Filename),
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlateform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlateform->Transform = Transform;
	MyPlateform->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	auto Movement = MyPlateform->AddComponent<Plateform>(
		Math::Transform(MyPlateform->Transform.Position)
		, Math::Transform(MyPlateform->Transform.Position + End), true);
	Movement->SetSpeed(Speed);

	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyPlateform));
}

void GameFactory::CreateLights(DirectX::XMFLOAT3 Pos, DirectX::XMFLOAT3 Specular, DirectX::XMFLOAT3 Roughness, float Intensity, float InnerRadius, float OuterRadius)
{
	auto& CurrentScene = PM3D::CMoteurWindows::GetInstance().GetScene();

	CurrentScene.LightManager.AmbientColor = { 0.5f };

	auto DirLight = new ADirectionalLight;
	DirLight->GetLight()->Direction = { 0.f, 4.f, 0.f };
	DirLight->GetLight()->Color = { 0.f, 1.f, 1.f };
	CurrentScene.AddActor(std::unique_ptr<Pitbull::Actor>(DirLight));

	auto ALightRed = new APointLight;
	auto LightRed = ALightRed->GetLight();
	LightRed->Position = { 0.f, 5.f, 0.f };
	LightRed->Color = { 1.0f, 0.0f, 0.0f };
	LightRed->Range = 20.f;
	CurrentScene.AddActor(std::unique_ptr<Pitbull::Actor>(ALightRed));

	auto ALightBlue = new APointLight;
	auto LightBlue = ALightBlue->GetLight();
	LightBlue->Position = { -40.f, 5.f, 0.f };
	LightBlue->Color = { 0.f, 0.0f, 1.0f };
	LightBlue->Range = 20.f;
	CurrentScene.AddActor(std::unique_ptr<Pitbull::Actor>(ALightBlue));

	auto ALightGreen = new APointLight;
	auto LightGreen = ALightGreen->GetLight();
	LightGreen->Position = { 40.f, 5.f, 0.f };
	LightGreen->Color = { 0.0f, 1.0f, 0.0f };
	LightGreen->Range = 20.f;
	CurrentScene.AddActor(std::unique_ptr<Pitbull::Actor>(ALightGreen));

	auto ASpotRed = new ASpotLight;
	auto SpotRed = ASpotRed->GetLight();
	SpotRed->Position = { 0.f, 5.f, -40.f };
	SpotRed->Direction = { -30.f, 0.f, -40.f };
	SpotRed->Color = { 1.0f, 0.0f, 0.0f };
	SpotRed->Range = 20.f;
	CurrentScene.AddActor(std::unique_ptr<Pitbull::Actor>(ASpotRed));
}

void GameFactory::CreateSkyBox(Math::Transform* ToFollow)
{
	PM3D::CMoteurWindows::GetInstance().GetScene().AddSkyBox(
		std::unique_ptr<Pitbull::Actor>(std::move(new Skybox{
			ToFollow
			, PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\sky\\sky.OMB")
			, PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhongSkyBox.fx")
		}))
	);
}


void GameFactory::CreateCheckPoint(Math::Transform Transform)
{
	auto MyCheckPoint = Pitbull::Actor::New("CheckPoint");
	MyCheckPoint->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\checkPoint\\star.OMB"),
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));

	MyCheckPoint->Transform = Transform;
	MyCheckPoint->AddComponent<CheckPoint>();

	auto CheckPointCollider = [](const Contact& Contact) -> void {
		if (Contact.FirstActor->Name == "CheckPoint" && Contact.SecondActor->Name == "Player" && !Contact.FirstActor->GetComponent<CheckPoint>()->IsVisited)
		{
			Contact.SecondActor->GetComponent<Player>()->SetSpawnPos(Contact.FirstActor->Transform.Position);
			Contact.FirstActor->GetComponent<CheckPoint>()->IsVisited = true;
			Contact.FirstActor->GetComponent<MeshRenderer>()->Mesh = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\checkPoint\\green_star.OMB");
		}
		else if (Contact.FirstActor->Name == "Player" && Contact.SecondActor->Name == "CheckPoint" && !Contact.SecondActor->GetComponent<CheckPoint>()->IsVisited)
		{
			Contact.FirstActor->GetComponent<Player>()->SetSpawnPos(Contact.SecondActor->Transform.Position);
			Contact.SecondActor->GetComponent<CheckPoint>()->IsVisited = true;
			Contact.SecondActor->GetComponent<MeshRenderer>()->Mesh = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\checkPoint\\green_star.OMB");
		}
	};

	auto Collider = MyCheckPoint->AddComponent<CapsuleCollider>(0.4f, 0.01f, PhysicMaterial{ 0.5f, 0.5f, 0.2f });
	Collider->OnContactCallBack = CheckPointCollider;

	MyCheckPoint->AddComponent<RigidBody>(RigidBody::RigidActorType::Static, true);

	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyCheckPoint));
}

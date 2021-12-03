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

// Gameplay components
#include "gameplay/Plateform.h"
#include "gameplay/Player.h"

void GameFactory::LoadLevel()
{
	CreateTerrain();
	CreateEnemy();
	CreatePlayer();
	CreatePlatform(Math::Vec3f(0, -2.f, 0), Math::Vec3f(20.f, 1.f, 20.f));
	CreateMobilePlatform(Math::Vec3f(10.f, 10.f, 0), Math::Vec3f(5.f, 1.f, 1.f), Math::Vec3f(10, 0, 0), L".\\modeles\\plateform\\plateformRouge.OMB");
	CreateMobilePlatform(Math::Vec3f(10.f, 10.f, 20.f), Math::Vec3f(1.f, 1.f, 1.f), Math::Vec3f(-10, 0, 0), L".\\modeles\\plateform\\plateformGlace.OMB");
	CreateLights();
}

void GameFactory::CreateTerrain()
{
	auto Terrain = std::unique_ptr<ATerrain>(new ATerrain{
			L".\\modeles\\Heightmap.bmp",
			{1, 0.3f, 1},
			PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhongTerrain.fx"),
			PhysicMaterial{ 0.5f, 0.5f, 0.2f}
		});
	Terrain->Texture1 = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetTexture(L".\\modeles\\gazon.dds");
	Terrain->Texture2 = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetTexture(L".\\modeles\\roche.dds");
	Terrain->Texture3 = PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetTexture(L".\\modeles\\chemin.dds");
	Terrain->Transform.Position = { 0.f, -50.f, 0.f };
	Terrain->Transform.Scale.x = 2.f;
	Terrain->Transform.Scale.z = 2.f;
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(Terrain), true);
}

void GameFactory::CreatePlayer()
{
	auto MyPlayer = Pitbull::Actor::New();
	MyPlayer->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\ball3\\ball.OMB"), 
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlayer->AddComponent<Player>(DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f));
	auto PlayerCam = MyPlayer->AddComponent<Camera>(DirectX::XMVectorSet(0.0f, 2.0f, 10.0f, 1.0f),
	                                                DirectX::XMVectorSet(0.0f, 0.4f, -1.0f, 1.0f),
	                                                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f),
	                                                &PM3D::CMoteurWindows::GetInstance().GetMatView(),
	                                                &PM3D::CMoteurWindows::GetInstance().GetMatProj(),
	                                                &PM3D::CMoteurWindows::GetInstance().GetMatViewProj());

	PM3D::CMoteurWindows::GetInstance().GetScene().SetCurrentCamera(PlayerCam);

	MyPlayer->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 0.2f });
	auto PlayerBody = MyPlayer->AddComponent<RigidBody>(RigidBody::RigidActorType::Dynamic);
	PlayerBody->SetMass(10.f);

	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyPlayer));
}

void GameFactory::CreateEnemy()
{
	auto Ennemy = Pitbull::Actor::New();
	Ennemy->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\cube\\cube.OMB"), 
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	Ennemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f }, Math::Vec3f{ 2.f, 0.f, 0.0f });
	Ennemy->AddComponent<SphereCollider>(1.0f, PhysicMaterial{ 0.5f, 0.5f, 1.0f }, Math::Vec3f{ -2.f, 0.f, 0.0f });
	Ennemy->Transform.Position.y = 0.f;
	Ennemy->Transform.Position.z = -7.f;
	Ennemy->Transform.Position.x = 1.f;
	auto body = Ennemy->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	Ennemy->AddComponent<Plateform>(
		Math::Transform(Ennemy->Transform.Position, Math::Quaternion(-physx::PxHalfPi, Math::Vec3f(0, 1, 0)))
		, Math::Transform(Ennemy->Transform.Position + Math::Vec3f(10, 0, 0), Math::Quaternion(physx::PxHalfPi, Math::Vec3f(0, 1, 0))), true);
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(Ennemy));
}

void GameFactory::CreatePlatform(Math::Vec3f Pos, Math::Vec3f Scale, PhysicMaterial Material)
{
	auto MyPlateform = Pitbull::Actor::New();
	MyPlateform->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(L".\\modeles\\platform\\plateform.OMB"),
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlateform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlateform->Transform.Position = Pos;
	MyPlateform->Transform.Scale = Scale;

	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyPlateform));
}

void GameFactory::CreateMobilePlatform(Math::Vec3f Pos, Math::Vec3f Scale, Math::Vec3f End, const wchar_t* Filename,
	PhysicMaterial Material)
{
	auto MyPlateform = Pitbull::Actor::New();
	MyPlateform->AddComponent<MeshRenderer>(PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetMesh(Filename),
		PM3D::CMoteurWindows::GetInstance().GetResourcesManager().GetShader(L".\\shaders\\MiniPhong.fx"));
	MyPlateform->AddComponent<BoxCollider>(Math::Vec3f{ 7.f, 0.20f, 7.f }, Material);
	MyPlateform->Transform.Position = Pos;
	MyPlateform->Transform.Scale = Scale;
	MyPlateform->AddComponent<RigidBody>(RigidBody::RigidActorType::Kinematic);
	MyPlateform->AddComponent<Plateform>(
		Math::Transform(MyPlateform->Transform.Position)
		, Math::Transform(MyPlateform->Transform.Position + End), true);
	PM3D::CMoteurWindows::GetInstance().GetScene().AddActor(std::move(MyPlateform));
}

void GameFactory::CreateLights()
{
	PM3D::CMoteurWindows::GetInstance().GetScene().LightConfig.SetAmbient(AmbientLight{ 0.5f, 0.5f, 0.5f });
	PointLight Light;
	Light.Intensity = 4.0f;
	Light.Position = { 0.f, 20.f, 0.f };
	Light.Specular = { 0.5f, 0.5f, 0.5f };
	Light.Roughness = { 0.5f, 0.5f, 0.5f };
	PM3D::CMoteurWindows::GetInstance().GetScene().LightConfig.AddPointLight(Light);
}